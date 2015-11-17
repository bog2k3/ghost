/*
 * main.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#define DUMMY_SQL_SOCKET

#include "listFile.h"

#include "../common/configFile.h"
#include "../common/cmdLine.h"
#include "../common/log.h"
#include "../common/dbLabels.h"
#ifdef DUMMY_SQL_SOCKET
#include "../common/DummySQLSock.h"
#else
#include "../common/SQLSock.h"
#endif
#include "../common/strCompare.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>

constexpr float idWordNorm_thresh = 0.55f;	// threshold for StrComp::Result::identicalWordsNormalized
constexpr float wordResemb_thresh = 0.75f;	// threshold for StrComp::Result::relativeWordResemblance

// numele argumentelor de pe cmd line:
struct {
	const std::string table {"table"};
	const std::string configPath {"config"};
	const std::string listePath {"lPath"};
} cmdLine;

// numele parametrilor din fisierul de config:
struct {
	const std::string dbURI {"URI"};
	const std::string dbUser {"user"};
	const std::string dbPassw {"passw"};
	const std::string dbName {"database"};
} config;

void maimutareste(ISQLSock &sock, std::string const& tabel, std::string const& listePath) {
	// 1. incarcam listele:
	listFile lf = loadListFile(listePath+'/'+tabel);
	if (lf.io_result != listFile::IO_OK) {
		ERROR("maimuța nu poate deschide listele!!! \"" << listePath+'/'+tabel << "\"");
		return;
	}

	// 2. cerem meciurile netraduse:
	auto res = sock.doQuery(
			"SELECT "+
			dbLabels.echipa1+","+
			dbLabels.echipa2+","+
			dbLabels.statusTraduceri+","+
			dbLabels.data +
			" FROM " + tabel +
			" WHERE " + dbLabels.statusTraduceri + " != 0"+
			" ORDER BY " + dbLabels.statusTraduceri + " ASCENDING");

	struct meciInfo {
		std::string echipa1;
		std::string echipa2;
		std::string data;
		int statusTrad;
	};

	std::vector<std::pair<std::string, std::string>> dubioase;

	// 3. parcurgem meciurile netraduse si incercam sa gasim traduceri:
	while (res->next()) {
		meciInfo crt;
		crt.echipa1 = res->getString(dbLabels.echipa1);
		crt.echipa2 = res->getString(dbLabels.echipa2);
		crt.data = res->getString(dbLabels.data);
		crt.statusTrad = res->getInt(dbLabels.statusTraduceri);

		// cautam meciuri simultane ca sa incercam sa traducem echipele:
		auto res2 = sock.doQuery(
			"SELECT "+
			dbLabels.echipa1+","+
			dbLabels.echipa2+","+
			dbLabels.statusTraduceri+
			" FROM " + tabel +
			" WHERE " + dbLabels.data + " = \""+crt.data+"\" "+
			" AND " + dbLabels.statusTraduceri + " != 3"+
			" ORDER BY " + dbLabels.statusTraduceri + " ASCENDING");	// vrem echipele traduse (status=0) la inceput daca e posibil

		/* !!! algoritmu descris aici e mai vechi, nu e chiar exact implementat, dar cat de cat:
		 *
		 * 1. punem toate meciurile returnate in res2 intr-un vector
		 *		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 *  !!! PRESUPUNEM CA ECHIPELE DIN ORICE MECI SUNT SORTATE IN ORDINE ALFABETICA (ECHIPA1 < ECHIPA2)
		 *		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 * 2. verificam care dintre echipe nu e tradusa (dupa status)
		 * 3. parcurgem vectorul si in cazul in care una dintre echipe este tradusa, eliminam meciurile care au echipa respectiva diferita
		 * 4a. DACA 1 echipa nu e tradusa:
		 * 		- comparam aproximativ echipa de tradus cu echipa2 din fiecare meci din vector; daca similaritatea e suficienta,
		 * 			consideram ca e aceeasi echipa (un threshold de similaritate)
		 * 		- adaugam intr-o lista temporara
		 * 4b. DACA nici una din echipe nu e tradusa:
		 * 		- la fel, doar ca fiecare dintre echipe trebuie sa depaseasca thresh-ul de similaritate simultan
		 * 5. parcurgem lista/listele temporara si incercam sa identificam in ea un element care exista in lista de echipe;
		 * 6a. DACA am gasit un element din lista de echipe, le adaugam pe celelalte (care nu sunt traduse) ca alternative la echipa respectiva
		 * 		in lista
		 * 6b. ALTFEL alegem cel mai lung element sa fie cheie, il introducem pe o linie noua in lista de echipe, si pe celelalte ca alternative
		 *
		 * 7. DACA nici o alta echipa nu a fost similara (caz singular), trimitem un email cu echipa respectiva (eventual concatenam toate
		 * 		echipele care nu au putut fi traduse intr-un email)
		 */


		while (res2->next()) {
			meciInfo r2;
			r2.echipa1 = res2->getString(dbLabels.echipa1);
			r2.echipa2 = res2->getString(dbLabels.echipa2);
			r2.data = res2->getString(dbLabels.data);
			r2.statusTrad = res2->getInt(dbLabels.statusTraduceri);

			switch (crt.statusTrad) {
			case 3:
				// nici o echipa din crt nu e tradusa, aici e cam naspa
				break;
			case 2:
			case 1:
				// o echipa din crt nu e tradusa
				std::string* pTrad = crt.statusTrad == 1 ? &crt.echipa2 : &crt.echipa1;
				std::string* pNetrad = crt.statusTrad == 1 ? &crt.echipa1 : &crt.echipa2;
				if (r2.statusTrad == 0) {
					// ambele echipe din r2 sunt traduse, de vis :-)
					std::string *pEchiv = (*pTrad == r2.echipa1) ? r2.echipa2 : ((*pTrad == r2.echipa2) ? r2.echipa1 : nullptr);
					if (pEchiv) {
						// inseamna ca pNetrad trebuie sa fie echivalenta cu r2.echipa1 sau r2.echipa2
						StrComp comp(*pNetrad, *pEchiv);
						auto cstat = comp.getStats();
						if ((cstat.identicalWordsNormalized > idWordNorm_thresh) ||
							(cstat.relativeWordResemblance > wordResemb_thresh)) {
							// OK
							lf.addNewAlias(*pEchiv, *pNetrad);
						} else {
							// e dubios, nu prea se potriveste, dam warning pe mail
							dubioase.push_back(std::make_pair(*pEchiv, *pNetrad));
							// adaugam si ca alias totusi, in caz ca e naspa doar stergem, nu stam sa le adaugam de mana pe toate dubioase
							lf.addNewAlias(*pEchiv, *pNetrad);
						}
					} else
						continue; // r2 nu e acelasi meci, chiar daca e in acelasi timp
				} else {
					// una dintre ecihpele din r2 nu e tradusa
					std::string* pR2Trad = r2.statusTrad == 1 ? &r2.echipa2 : &r2.echipa1;
					std::string* pR2Netrad = r2.statusTrad == 1 ? &r2.echipa1 : &r2.echipa2;
					if (*pR2Trad == *pTrad) {
						// avem o echipa comuna si aceeasi data => si cealalata TREBUIE sa fie aceeasi
						StrComp scomp(*pR2Netrad, *pNetrad);
						auto cstat = scomp.getStats();
						if ((cstat.identicalWordsNormalized > idWordNorm_thresh) ||
							(cstat.relativeWordResemblance > wordResemb_thresh)) {
							// OK
							lf.addNewAlias(*pNetrad, *pR2Netrad);
						} else {
							// e dubios, nu prea se potriveste, dam warning pe mail
							dubioase.push_back(std::make_pair(*pNetrad, *pR2Netrad));
							// adaugam si ca alias totusi, in caz ca e naspa doar stergem, nu stam sa le adaugam de mana pe toate dubioase
							lf.addNewAlias(*pNetrad, *pR2Netrad);
						}
						// adica => *pR2Netrad == *pNetrad    -> avem inregistrare noua aici, alegem pe cea mai lunga ca cheie
						// verificam cu StrComp si daca potrivirea e mica, dam warning pe mail
					} else {
						// s-ar putea ca fiecare echipa tradusa sa fie echivalenta cu cea netradusa, verificam:
						// A: pTrad vs pR2Netrad
						// B: pNetrad vs pR2Trad
						StrComp scA(*pTrad, *pR2Netrad);
						StrComp scB(*pR2Trad, *pNetrad);
						auto statA = scA.getStats();
						auto statB = scB.getStats();
						decltype(statA) *statBig = &statA, *statSmall = statB;
						// verificam daca sunt in ordinea corecta (statBig se potriveste mai mult decat statSmall)
						// si interschimbam daca nu:
						if (statBig->identicalWordsNormalized == 0) {
							if (statBig->relativeWordResemblance < statSmall->relativeWordResemblance) {
								decltype(statBig) aux = statBig;
								statBig = statSmall;
								statSmall = aux;
							}
						} else if (statBig->identicalWordsNormalized < statSmall->identicalWordsNormalized) {
							decltype(statBig) aux = statBig;
							statBig = statSmall;
							statSmall = aux;
						}
						if (statBig->identicalWordsNormalized < idWordNorm_thresh) {
							if (statBig->relativeWordResemblance < wordResemb_thresh)
								// nu pare sa fie acelasi meci, trecem mai departe
								continue;
						}
						// pare sa fie acelasi meci, TREBUIE sa se potriveasca si celelalte echipe, altfel e dubios:
						if (statSmall)
					}
				}
				break;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	LOGPREFIX("maimuță");
	std::locale::global(std::locale(""));
	std::cout.imbue(std::locale(""));

	std::map<std::string, std::string> cmdOpts;
	cmdOpts[cmdLine.listePath] = ".";
	if (!parseCommandLine(argc, argv, cmdOpts, {cmdLine.table, cmdLine.configPath, cmdLine.listePath})) {
		ERROR("Linie de comanda invalida!");
		ERROR("Exemplu linie de comanda valida:\n" <<
				"maimutza --table fotbal --config ~/.maimutza.config --lPath path/to/liste");
		return -1;
	}
	std::map<std::string, std::string> configOpts;
	if (!cmdOpts["config"].empty())
		if (!parseConfigFile(cmdOpts["config"], configOpts, {config.dbURI, config.dbUser, config.dbPassw, config.dbName})) {
			ERROR("maimuța nu poate continua (configurare incompleta)");
			return -1;
		}

	ISQLSock *pSock;
#ifdef DUMMY_SQL_SOCKET
	DummySQLSock dummySqlSock("test-maimuța");
	pSock = &dummySqlSock;
#else
	SQLSock sqlSock;
	pSock = &sqlSock;
#endif
	if (!pSock->connect(configOpts[config.dbURI], configOpts[config.dbUser], configOpts[config.dbPassw])
			|| !pSock->setDB(configOpts[config.dbName])) {
		ERROR("Nu s-a putut initializa conexiunea la baza de date, shefu'!");
		return -1;
	}

	LOGLN("maimuța incepe loop-ul de cautat echipe netraduse si adaugat in liste...");

	while (true) { // maimuța e nemuritoare!!! muhahaha !!
		maimutareste(*pSock, cmdOpts[cmdLine.table], cmdOpts[cmdLine.listePath]);
		sleep(10); // maimuța doarme 10 secunde intre incercari

		break;
	}

	return 0;
}



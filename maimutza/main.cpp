/*
 * main.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#define DUMMY_SQL_SOCKET

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

// numele argumentelor de pe cmd line:
struct {
	const std::string table {"table"};
	const std::string configPath {"config"};
} cmdLine;

// numele parametrilor din fisierul de config:
struct {
	const std::string dbURI {"URI"};
	const std::string dbUser {"user"};
	const std::string dbPassw {"passw"};
	const std::string dbName {"database"};
} config;

void faQuery(ISQLSock &sock, std::string const& tabel) {
	auto res = sock.doQuery(
			"SELECT "+
			dbLabels.echipa1+","+
			dbLabels.echipa2+","+
			dbLabels.statusTraduceri+","+
			dbLabels.data +
			" FROM " + tabel +
			" WHERE " + dbLabels.statusTraduceri + " != 0"+
			" ORDER BY " + dbLabels.statusTraduceri + " ASCENDING");

	std::map<std::string, std::string> traduceriCurente; // aici stocam traducerile noi pe care le facem la pasul asta, la sfarsit le scriem in liste
				// key - stringul netradus; val - stringul tradus

	struct meciInfo {
		std::string echipa1;
		std::string echipa2;
		std::string data;
		int statusTrad;
	};

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

		/*
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
					if (*pTrad == r2.echipa1) {
						// inseamna ca pNetrad e echivalenta cu r2.echipa2
						// verificam cu StrComp si daca potrivirea e mica, dam warning pe mail
					} else if (*pTrad == r2.echipa2) {
						// inseamna ca pNetrad e echivalenta cu r2.echipa1
						// verificam cu StrComp si daca potrivirea e mica, dam warning pe mail
					} else
						continue; // r2 nu e acelasi meci, chiar daca e in acelasi timp
				} else {
					// una dintre ecihpele din r2 nu e tradusa
					std::string* pR2Trad = r2.statusTrad == 1 ? &r2.echipa2 : &r2.echipa1;
					std::string* pR2Netrad = r2.statusTrad == 1 ? &r2.echipa1 : &r2.echipa2;
					if (*pR2Trad == *pTrad) {
						// avem o echipa comuna si aceeasi data => si cealalata trebuie sa fie aceeasi
						// adica => *pR2Netrad == *pNetrad    -> avem inregistrare noua aici, alegem pe cea mai lunga ca cheie
						// verificam cu StrComp si daca potrivirea e mica, dam warning pe mail
					}
				}
				break;
			}

//			simultane.push_back(r2);
		}
	}
}

int main(int argc, char* argv[]) {
	LOGPREFIX("maimuță");
	std::locale::global(std::locale(""));
	std::cout.imbue(std::locale(""));

	std::map<std::string, std::string> cmdOpts;
	cmdOpts["lPath"] = ".";
	if (!parseCommandLine(argc, argv, cmdOpts, {cmdLine.table, cmdLine.configPath})) {
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
		faQuery(*pSock, cmdOpts[cmdLine.table]);
		sleep(10); // maimuța doarme 10 secunde intre incercari

		break;
	}

	return 0;
}



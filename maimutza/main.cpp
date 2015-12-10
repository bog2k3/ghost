/*
 * main.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

//#define DUMMY_SQL_SOCKET

#include "listFile.h"

#include "../common/configFile.h"
#include "../common/cmdLine.h"
#include "../common/log.h"
#include "../common/dbLabels.h"
#include "../common/assert.h"
#ifdef DUMMY_SQL_SOCKET
#include "../common/DummySQLSock.h"
#else
#include "../common/SQLSock.h"
#endif
#include "../common/strCompare.h"
#include "../common/strManip.h"
#include "../common/eMailer.h"
#include "../common/sanitize.h"
#include "../common/dir.h"
#include "../common/wordFreq.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>

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

	const std::string mailUser {"mail-user"};
	const std::string mailPassw {"mail-passw"};
	const std::string mailServer {"mail-server"};
	const std::string mailSenderAddr {"mail-addr"};
	const std::string mailFromName {"mail-from"};
	const std::string mailSignature {"mail-signature"};
	const std::string mailDestList {"mail-dest"};
} config;

struct meciInfo {
	std::string echipa1;
	std::string echipa2;
	std::string data;
	int statusTrad;
};

EMailer *pEmailer {nullptr};
std::vector<std::string> emailRecipients;

double fitness(StrComp::Result const& res) {
	return res.identicalWords * res.relativeWordResemblance;
}

bool acceptCondition(StrComp::Result const& res) {
	constexpr float accept_thresh_idrwr = 0.8f;
	return res.identicalWords >= 1 && fitness(res)  >= accept_thresh_idrwr;
}

bool processNetrad(std::string& a1, std::string& a2,
					std::string& b1, std::string& b2,
					listFile &lf,
					WordFreqMap const& fmap,
					std::vector<std::pair<std::string, std::string>> &suspecte) {
	sanitize(a1); sanitize(a2);
	sanitize(b1); sanitize(b2);
	StrComp scA(a1, b1);
	StrComp scB(a2, b2);
	auto statA = scA.getStats(&fmap);
	auto statB = scB.getStats(&fmap);
	decltype(statA) *statBig = &statA, *statSmall = &statB;
	// verificam daca sunt in ordinea corecta (statBig se potriveste mai mult decat statSmall)
	// si interschimbam daca nu:
	if (fitness(*statBig) < fitness(*statSmall)) {
		decltype(statBig) aux = statBig;
		statBig = statSmall;
		statSmall = aux;
	}
	if (!acceptCondition(*statBig)) {
		// nu pare sa fie acelasi meci, trecem mai departe
		return false;
	}
	// pare sa fie acelasi meci, TREBUIE sa se potriveasca si celelalte echipe, altfel e dubios:
	if (!acceptCondition(*statSmall)) {
		std::string &s1r = statSmall == &statA ? statA.s1 : statB.s1;
		std::string &s2r = statSmall == &statA ? statA.s2 : statB.s2;
		suspecte.push_back(std::make_pair(s1r, s2r));
		LOGLN("SUSPECT: " << s1r << "   ~   " << s2r << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
	lf.addNewAlias(a1, b1);
	LOGLN(b1 << "  ->  " << a1);
	lf.addNewAlias(a2, b2);
	LOGLN(b2 << "  ->  " << a2);
	return true;
}

bool process(meciInfo& crt, meciInfo& r2, listFile &lf,
		WordFreqMap const& fmap,
		std::vector<std::pair<std::string, std::string>> &suspecte,
		std::vector<meciInfo> *postponed) {
	switch (crt.statusTrad) {
	case 3:
		// nici o echipa din crt nu e tradusa, aici e cam naspa
		if (postponed) {
			postponed->push_back(crt);
			return true;
		}
		// try a1 vs b1 & a2 vs b2
		// then a1 vs b2 & a2 vs b1
		for (int cross=0; cross<2; cross++) {
			if (cross) {
				if (processNetrad(crt.echipa1, crt.echipa2, r2.echipa1, r2.echipa2, lf, fmap, suspecte))
					return true;
			} else if (processNetrad(crt.echipa2, crt.echipa1, r2.echipa1, r2.echipa2, lf, fmap, suspecte))
				return true;
		}
		return false;
		break;
	case 2:
	case 1:
		// o echipa din crt nu e tradusa
		std::string* pTrad = crt.statusTrad == 1 ? &crt.echipa2 : &crt.echipa1;
		std::string* pNetrad = crt.statusTrad == 1 ? &crt.echipa1 : &crt.echipa2;
		if (r2.statusTrad == 0) {
			// ambele echipe din r2 sunt traduse, de vis :-)
			std::string *pEchiv = (*pTrad == r2.echipa1) ? &r2.echipa2 : ((*pTrad == r2.echipa2) ? &r2.echipa1 : nullptr);
			if (pEchiv) {
				// inseamna ca pNetrad trebuie sa fie echivalenta cu r2.echipa1 sau r2.echipa2
				sanitize(*pNetrad);
				sanitize(*pEchiv);
				StrComp comp(*pNetrad, *pEchiv);
				auto cstat = comp.getStats(&fmap);
				lf.addNewAlias(*pEchiv, *pNetrad);
				LOGLN(*pNetrad << "  ->  " << *pEchiv);
				if (!acceptCondition(cstat)) {
					// e dubios, nu prea se potriveste, dam warning pe mail
					suspecte.push_back(std::make_pair(*pEchiv, *pNetrad));
					LOGLN("SUSPECT: " << *pEchiv << "   ~   " << *pNetrad << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				}
				return true;
			} else
				return false; // r2 nu e acelasi meci, chiar daca e in acelasi timp
		} else {
			// una dintre ecihpele din r2 nu e tradusa
			std::string* pR2Trad = r2.statusTrad == 1 ? &r2.echipa2 : &r2.echipa1;
			std::string* pR2Netrad = r2.statusTrad == 1 ? &r2.echipa1 : &r2.echipa2;
			if (*pR2Trad == *pTrad) {
				// avem o echipa comuna si aceeasi data => si cealalata TREBUIE sa fie aceeasi
				// adica => *pR2Netrad == *pNetrad
				sanitize(*pR2Netrad);
				sanitize(*pNetrad);
				StrComp scomp(*pR2Netrad, *pNetrad);
				auto cstat = scomp.getStats(&fmap);
				lf.addNewAlias(*pNetrad, *pR2Netrad);
				LOGLN(*pR2Netrad << "  ->  " << *pNetrad);
				if (!acceptCondition(cstat)) {
					// e dubios, nu prea se potriveste, dam warning pe mail
					suspecte.push_back(std::make_pair(*pNetrad, *pR2Netrad));
					LOGLN("SUSPECT: " << *pNetrad << "   ~   " << *pR2Netrad << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				}
				return true;
			} else {
				// s-ar putea ca fiecare echipa tradusa sa fie echivalenta cu cea netradusa, verificam:
				// A: pTrad vs pR2Netrad
				// B: pNetrad vs pR2Trad
				return processNetrad(*pTrad, *pNetrad, *pR2Netrad, *pR2Trad, lf, fmap, suspecte);
			}
		}
		break;
	}
	assertDbg(false); // nu ar trebui sa ajunga aici
	return false;
}

void maimutareste(ISQLSock &sock, std::string const& tabel, std::string const& listePath) {
	// 1. incarcam listele:
	auto listFilePath = listePath+'/'+tabel;
	listFile lf = loadListFile(listFilePath);

	// 2. build freq map:
	WordFreqMap fmap;
	for (auto line : lf.lines) {
		std::vector<std::string> words;
		for (auto alias : line) {
			auto aliasTok = strSplit(alias, ' ');
			for (auto tok : aliasTok)
				if (std::find(words.begin(), words.end(), tok) == words.end())
					words.push_back(tok);
		}
		fmap.addWordList(words);
	}
	if (lf.io_result != listFile::IO_OK) {
		ERROR("maimuța nu poate deschide listele!!! \"" << listePath+'/'+tabel << "\"");
		return;
	}

	// 3. facem un sanity test pe baza de date:
	auto res = sock.doQuery(
			"SELECT "+
			dbLabels.echipa1+","+
			dbLabels.echipa2+","+
			dbLabels.statusTraduceri+","+
			dbLabels.data +
			" FROM " + tabel +
			" WHERE " + dbLabels.echipa1 + "=" + dbLabels.echipa2);
	if (res && res->rowsCount()) {
		ERROR("Apar meciuri in DB cu aceeasi echipa vs ea insasi!!!");
		if (pEmailer) {
			std::stringstream ss;
			ss << "Am gasit meciuri invalide in DB:\r\n\r\n";
			while (res->next()) {
				ss << res->getString(dbLabels.echipa1) << "  vs  ";
				ss << res->getString(dbLabels.echipa2) << "  ";
				ss << "[status: " << res->getString(dbLabels.statusTraduceri) <<"]  ";
				ss << res->getString(dbLabels.data) << "\r\n";
			}
#ifndef DEBUG
			pEmailer->send(emailRecipients, "Meciuri invalide in DB!", ss.str());
#endif
		}
	}

	// 4. cerem meciurile netraduse:
	res = sock.doQuery(
			"SELECT "+
			dbLabels.echipa1+","+
			dbLabels.echipa2+","+
			dbLabels.statusTraduceri+","+
			dbLabels.data +
			" FROM " + tabel +
			" WHERE " + dbLabels.statusTraduceri + " != 0"+
			" ORDER BY " + dbLabels.statusTraduceri + " ASC");

	if (!res || !res->rowsCount()) {
		LOGLN("Nu exista meciuri netraduse!! Maimutza trage un pui de somnic....");
		return;
	}

	std::vector<std::pair<std::string, std::string>> suspecte;
	std::vector<meciInfo> meciuriSingulare;
	std::vector<meciInfo> postponed;

	for (int pas=1; pas <= 2; pas++) {

		std::vector<meciInfo> crtList;
		// la pas==1 cerem meciurile din db netraduse
		// la pas==2 folosim meciurile postponed

		// 3. parcurgem meciurile netraduse si incercam sa gasim traduceri:
		if (pas == 1)
			while (res->next()) {
				meciInfo crt;
				crt.echipa1 = res->getString(dbLabels.echipa1);
				crt.echipa2 = res->getString(dbLabels.echipa2);
				crt.data = res->getString(dbLabels.data);
				crt.statusTrad = res->getInt(dbLabels.statusTraduceri);

				crtList.push_back(crt);
			}
		else
			crtList.swap(postponed);

		LOGLN("pas " << pas << "; " << crtList.size() << " meciuri netraduse");

//#ifdef DEBUG
//		int crtIndex = 0;
//#endif
		for (auto crt : crtList) {
//#ifdef DEBUG
//			LOGLN(crtIndex++);
//#endif
			// cautam meciuri simultane ca sa incercam sa traducem echipele:
			auto res2 = sock.doQuery(
				"SELECT "+
				dbLabels.echipa1+","+
				dbLabels.echipa2+","+
				dbLabels.statusTraduceri+
				" FROM " + tabel +
				" WHERE " + dbLabels.data + " = '"+crt.data+"' "+
				((pas == 1)
					? " AND " + dbLabels.statusTraduceri + " != 3"
					: "") +
				" ORDER BY " + dbLabels.statusTraduceri + " ASC");	// vrem echipele traduse (status=0) la inceput daca e posibil

			if ((!res2 || !res2->rowsCount()) && pas == 1) {
				postponed.push_back(crt);
#ifdef DEBUG
				if (pas == 2)
					LOGLN("!!! Nici un meci simultan cu " << crt.echipa1 << " vs " << crt.echipa2 << " " << crt.data);
#endif
				continue;
			}

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

			bool found = false;
			while (res2->next()) {
				meciInfo r2;
				r2.echipa1 = res2->getString(dbLabels.echipa1);
				r2.echipa2 = res2->getString(dbLabels.echipa2);
				r2.data = crt.data;
				r2.statusTrad = res2->getInt(dbLabels.statusTraduceri);

				found |= process(crt, r2, lf, fmap, suspecte, pas == 1 ? &postponed : nullptr);
			}

			if (!found) {
				if (pas == 1)
					postponed.push_back(crt);
				else {
					// daca a ajuns aici, inseamna ca nu s-a gasit nici un meci simultan care sa fie acceptabil, ceea ce e
					// DUBIOS RAU
					meciuriSingulare.push_back(crt);
				}
			}
		} // for crt
	} // for pas

#ifndef DEBUG
	if (saveListFile(listFilePath, lf) != listFile::IO_OK)
		ERROR("Nu am putut salva lista in " + listFilePath);
#endif

#ifndef DEBUG
	// dam mail cu echipele suspecte:
	if (suspecte.size()) {
		std::stringstream ss;
		if (suspecte.size()) {
			ss << "Traduceri suspecte >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n\r\n";
			for (auto d : suspecte) {
				ss << d.first << "   ?   " << d.second << "\r\n";
			}
		}
		if (meciuriSingulare.size()) {
			ss << "Meciuri SINGULARE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n";
			ss << "(adica nu mai exista altul simultan care sa se potriveasca)\r\n\r\n";
			for (auto s : meciuriSingulare)
				ss << s.echipa1 << " vs " << s.echipa2 << "  " << s.data << "\r\n";
		}
		if (!ss.eof())
			pEmailer->send(emailRecipients, "Suspecte & Singulare", ss.str());
	}
#endif
	LOGNP("\n\n");
	LOGLN("STATISTICI:");
	LOGLN(suspecte.size() << " traduceri suspecte.");
	LOGLN(meciuriSingulare.size() << " meciuri singulare.");
}

int main(int argc, char* argv[]) {
//	std::locale::global(std::locale(""));
//	std::cout.imbue(std::locale(""));

	std::map<std::string, std::string> cmdOpts;
	cmdOpts[cmdLine.listePath] = ".";
	if (!parseCommandLine(argc, argv, cmdOpts, {cmdLine.table, cmdLine.configPath, cmdLine.listePath})) {
		ERROR("Linie de comanda invalida!");
		ERROR("Exemplu linie de comanda valida:\n" <<
				"maimutza --table fotbal --config ~/.maimutza.config --lPath path/to/liste");
		return -1;
	}

	const char* pHomeDir = getenv("HOME");
	if (pHomeDir) {
		std::string homeDir(pHomeDir);
		if (!pathExists(homeDir+"/.maimutza")) {
			if (!mkDir(homeDir+"/.maimutza")) {
				ERROR("Nu am putut crea directorul ~/.maimutza\nLogurile nu vor fi salvate!!!");
			} else {
				std::ofstream fErrLog(std::string(pHomeDir) + "/.maimutza/error.log", std::ios::app);
				logger::setAdditionalErrStream(&fErrLog);
			}
		}
	} else {
		ERROR("Nu s-a putut accesa $HOME\nLogurile nu vor fi salvate!!!");
	}

	std::map<std::string, std::string> configOpts;
	if (!cmdOpts["config"].empty())
		if (!parseConfigFile(cmdOpts["config"], configOpts, {config.dbURI, config.dbUser, config.dbPassw, config.dbName,
				config.mailServer, config.mailUser, config.mailPassw, config.mailSenderAddr, config.mailDestList})) {
			ERROR("maimuța nu poate continua (configurare incompleta)");
			return -1;
		}

	pEmailer = new EMailer(configOpts[config.mailServer], configOpts[config.mailUser], configOpts[config.mailPassw],
			configOpts[config.mailSenderAddr], configOpts[config.mailFromName], configOpts[config.mailSignature]);
	emailRecipients = strSplit(configOpts[config.mailDestList], ',');

	ISQLSock *pSock;
#ifdef DUMMY_SQL_SOCKET
	DummySQLSock dummySqlSock("test-maimutza");
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
		LOGLN("Maimutza face un pas...");
		maimutareste(*pSock, cmdOpts[cmdLine.table], cmdOpts[cmdLine.listePath]);
#ifdef DEBUG
		break;
#endif
		LOGLN("doarme 10s...");
		sleep(10); // maimuța doarme 10 secunde intre incercari
	}

	delete pEmailer;

	return 0;
}



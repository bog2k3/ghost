/*
 * main.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "cupidData.h"
#include "../common/cmdLine.h"
#include "../common/configFile.h"
#include "../common/SQLSock.h"
#include "../common/log.h"
#include <unistd.h>

std::vector<cupidGameData> gameData;
std::vector<cupidPair> pairs;
std::map<std::string, cupidSiteData> mapSites;

void initializeazaChelneri();
bool initializeazaDB(SQLSock &socket, std::string const& URI, std::string const& user, std::string const& passw, std::string const& dbName);
void omoaraChelneri();
void faQueryul(SQLSock &socket, std::string tabel);
void gasestePerechi();
void plaseazaComenzile();
void curataDatele();

// to test shit, like curl, sql etc
void testCurl();
void testDB();

int main(int argc, char* argv[]) {
	LOGPREFIX("cupid");
	std::map<std::string, std::string> cmdOpts;
	if (!parseCommandLine(argc, argv, cmdOpts, {"table", "config"})) {
		ERROR("Linie de comanda invalida!");
		return -1;
	}
	std::map<std::string, std::string> configOpts;
	if (!cmdOpts["config"].empty())
		if (!parseConfigFile(cmdOpts["config"], configOpts, {"dbURI", "dbUser", "dbPassw", "dbName"})) {
			ERROR("cupid nu poate continua (configurare incompleta)");
			return -1;
		}

//	testCurl();

	initializeazaChelneri();

	SQLSock sqlSock;
	if (!initializeazaDB(sqlSock, configOpts["dbURI"], configOpts["dbUser"], configOpts["dbPassw"], configOpts["dbName"])) {
		ERROR("Nu s-a putut initializa conexiunea la baza de date, shefu'!");
		return -1;
	}

	LOGLN("cupid incepe loop-ul de match-making...");

	while (true) { // yes, Cupid lives forever! (or until it crashes)
		faQueryul(sqlSock, cmdOpts["table"]);
		gasestePerechi();
		plaseazaComenzile();
		sleep(1); // cupid sleeps for 1 second between cycles
		curataDatele();

		break;
	}
	omoaraChelneri();
	return 0;
}


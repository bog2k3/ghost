/*
 * main.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include <unistd.h>

#include "cupidData.h"
#include "cmdLine.h"
#include "../common/SQLSock.h"
#include "../common/log.h"

std::vector<cupidGameData> gameData;
std::vector<cupidPair> pairs;
std::map<std::string, cupidSiteData> mapSites;

void parseConfigFile(cmdLineOptions &opts);
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
	cmdLineOptions cmdOpts;
	parseCommandLine(cmdOpts, argc, argv);
	if (!cmdOpts.configFilePath.empty())
		parseConfigFile(cmdOpts);

//	testCurl();

	initializeazaChelneri();

	SQLSock sqlSock;
	if (!initializeazaDB(sqlSock, cmdOpts.dbURI, cmdOpts.dbUser, cmdOpts.dbPassw, cmdOpts.dbName)) {
		ERROR("Nu s-a putut initializa conexiunea la baza de date, shefu'!");
		return -1;
	}

	LOGLN("cupid incepe loop-ul de match-making...");

	while (true) { // yes, Cupid lives forever! (or until it crashes)
		faQueryul(sqlSock, cmdOpts.tableName);
		gasestePerechi();
		plaseazaComenzile();
		sleep(1); // cupid sleeps for 1 second between cycles
		curataDatele();

		break;
	}
	omoaraChelneri();
	return 0;
}


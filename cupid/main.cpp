/*
 * main.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include <unistd.h>

#include "cupidData.h"
#include "cmdLine.h"
#include "SQLSock.h"
#include "../cpplib/log.h"

std::vector<cupidGameData> gameData;
std::vector<cupidPair> pairs;
std::map<siteId, cupidSiteData> mapSites;

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
	LOGGER("cupid");
	cmdLineOptions cmdOpts;
	parseCommandLine(cmdOpts, argc, argv);
	if (!cmdOpts.configFilePath.empty())
		parseConfigFile(cmdOpts);

//	testCurl();

	initializeazaChelneri();

	SQLSock sqlSock;
	if (!initializeazaDB(sqlSock, cmdOpts.dbURI, cmdOpts.dbUser, cmdOpts.dbPassw, cmdOpts.dbName)) {
		ERROR("Could not initialize database connection!");
		return -1;
	}

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


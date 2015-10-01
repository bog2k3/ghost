/*
 * main.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include <unistd.h>

#include "cupidData.h"
#include "cmdLine.h"

std::vector<cupidGameData> gameData;
std::vector<cupidPair> pairs;
std::map<siteId, cupidSiteData> mapSites;

void initializeazaChelneri();
void initializeazaDB();
void omoaraChelneri();
void faQueryul(std::string tabel);
void gasestePerechi();
void plaseazaComenzile();
void curataDatele();

// to test shit, like curl, sql etc
void testCurl();
void testDB();

int main(int argc, char* argv[]) {
	cmdLineOptions cmdOpts;
	parseCommandLine(cmdOpts, argc, argv);

//	testCurl();

	initializeazaChelneri();
	initializeazaDB();

	return 0;

	while (true) { // yes, Cupid lives forever! (or until it crashes)
		faQueryul(cmdOpts.tableName);
		gasestePerechi();
		plaseazaComenzile();
		sleep(1); // cupid sleeps for 1 second between cycles
		curataDatele();

		break;
	}
	omoaraChelneri();
	return 0;
}


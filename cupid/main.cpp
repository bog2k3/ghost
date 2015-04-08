/*
 * main.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include <unistd.h>

#include "cupidData.h"

std::vector<cupidGameData> gameData;
std::vector<cupidPair> pairs;
std::map<siteId, cupidSiteData> mapSites;

void initializeazaChelneri();
void omoaraChelneri();
void faQueryul();
void gasestePerechi();
void plaseazaComenzile();
void curataDatele();

int main(int argc, char* argv[]) {
	initializeazaChelneri();
	while (true) { // yes, Cupid lives forever! (or until it crashes)
		faQueryul(); // astea trebuie sa se faca pe un tabel de sport - parametru de comanda la cupid?
		gasestePerechi();
		plaseazaComenzile();
		sleep(1); // cupid sleeps for 1 second between cycles
		curataDatele();
	}
	omoaraChelneri();
	return 0;
}


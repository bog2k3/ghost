/*
 * matching.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "cupidData.h"
#include "../cpplib/log.h"
#include <algorithm>

bool eCombinatie(float c1, float c2, double &out_raport) {
	out_raport = c2 - 1;
	return c1*c2 >= c1+c2;
}

void gasestePerechi() {
	// ne bazam pe faptul ca intrarile din DB sunt deja ordonate dupa meci, fiind grupate cele cu acelasi meci de pe site-uri diferite
	for (uint i=0; i<gameData.size(); i++) {
		uint j=i+1;
		for (; j<gameData.size(); j++) {
			if (gameData[j].internalGameId == gameData[i].internalGameId) {
				// am gasit o potentiala pereche (acelasi joc pe 2 site-uri)
				decltype(pairs) crtPairs;
				// acum vedem daca sunt cotele bune
				double raport = 0;
				// verificam toate combinatiile opuse:
				if (eCombinatie(gameData[i].cote[pariu::P1], gameData[j].cote[pariu::PX2], raport))
					crtPairs.push_back(cupidPair{i, pariu::P1, j, pariu::PX2, raport});
				if (eCombinatie(gameData[i].cote[pariu::P1X], gameData[j].cote[pariu::P2], raport))
					crtPairs.push_back(cupidPair{i, pariu::P1X, j, pariu::P2, raport});
				if (eCombinatie(gameData[i].cote[pariu::PX], gameData[j].cote[pariu::P12], raport))
					crtPairs.push_back(cupidPair{i, pariu::PX, j, pariu::P12, raport});
				if (eCombinatie(gameData[i].cote[pariu::PX2], gameData[j].cote[pariu::P1], raport))
					crtPairs.push_back(cupidPair{i, pariu::PX2, j, pariu::P1, raport});
				if (eCombinatie(gameData[i].cote[pariu::P2], gameData[j].cote[pariu::P1X], raport))
					crtPairs.push_back(cupidPair{i, pariu::P2, j, pariu::P1X, raport});
				if (eCombinatie(gameData[i].cote[pariu::P12], gameData[j].cote[pariu::PX], raport))
					crtPairs.push_back(cupidPair{i, pariu::P12, j, pariu::PX, raport});

				// sortam potentialele combinatii dupa cel mai bun raport de cote:
				std::sort(crtPairs.begin(), crtPairs.end(), [](cupidPair const& left, cupidPair const& rght) -> bool {
					double rat_left = gameData[left.index1].cote[left.p1] / gameData[left.index2].cote[left.p2];
					double rat_rght = gameData[rght.index1].cote[rght.p1] / gameData[rght.index2].cote[rght.p2];
					return rat_left > rat_rght;
				});
				// luam doar cea mai buna combinatie:
				if (crtPairs.size())
					pairs.push_back(crtPairs[0]);
			} else {
				// daca id-urile nu se mai potrivesc, inseamna ca am trecut la alt meci
				break;
			}
		}
		i = j;
	}
	if (pairs.size())
		LOGLN("Am gasit " << pairs.size() << "combinatii !!!")
	else
		LOGLN("NU am gasit combinatii :-(")
}

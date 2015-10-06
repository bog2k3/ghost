/*
 * matching.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "cupidData.h"
#include "../cpplib/log.h"

bool eCombinatie(float c1, float c2, float &out_raport) {
	// aici se intampla magia matematicii :-)
	return false;
}

void gasestePerechi() {
	// ne bazam pe faptul ca intrarile din DB sunt deja ordonate dupa meci, fiind grupate cele cu acelasi meci de pe site-uri diferite
	for (int i=0; i<gameData.size(); i++) {
		int j=i+1;
		for (; ; j++) {
			if (gameData[j].internalGameId == gameData[i].internalGameId) {
				// am gasit o potentiala pereche (acelasi joc pe 2 site-uri)
				// acum vedem daca sunt cotele bune
				float raport = 0;
				// verificam toate combinatiile opuse:
				if (eCombinatie(gameData[i].cota1, gameData[j].cota2x, raport))
					pairs.push_back(cupidPair{i, pariu::P1, j, pariu::PX2, raport});
				if (eCombinatie(gameData[i].cota1x, gameData[j].cota2, raport))
					pairs.push_back(cupidPair{i, pariu::P1X, j, pariu::P2, raport});
				if (eCombinatie(gameData[i].cotax, gameData[j].cota12, raport))
					pairs.push_back(cupidPair{i, pariu::PX, j, pariu::P12, raport});
				if (eCombinatie(gameData[i].cota2x, gameData[j].cota1, raport))
					pairs.push_back(cupidPair{i, pariu::PX2, j, pariu::P1, raport});
				if (eCombinatie(gameData[i].cota2, gameData[j].cota1x, raport))
					pairs.push_back(cupidPair{i, pariu::P2, j, pariu::P1X, raport});
				if (eCombinatie(gameData[i].cota12, gameData[j].cotax, raport))
					pairs.push_back(cupidPair{i, pariu::P12, j, pariu::PX, raport});
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

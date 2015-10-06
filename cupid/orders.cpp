/*
 * orders.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "cupidData.h"
#include "../cpplib/log.h"
#include <ostream>

std::ostream & operator <<(std::ostream& str, pariu p) {
	switch (p) {
	case pariu::P1:
		str << "1";
		break;
	case pariu::P1X:
		str << "1X";
		break;
	case pariu::PX:
		str << "X";
		break;
	case pariu::PX2:
		str << "X2";
		break;
	case pariu::P2:
		str << "2";
		break;
	case pariu::P12:
		str << "12";
		break;
	}
	return str;
}

void plaseazaComenzile() {
	for (uint i = 0; i < pairs.size(); i++) {
		LOGLN("COMBINATIE " << i << ": "
				<< gameData[pairs[i].index1].gameId << "(" << gameData[pairs[i].index1].siteId << ") {" << pairs[i].p1 << "}"
				<< " vs "
				<< gameData[pairs[i].index2].gameId << "(" << gameData[pairs[i].index2].siteId << ") {" << pairs[i].p2 << "}"
				);
	}
}


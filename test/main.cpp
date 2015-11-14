/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "../common/strManip.h"
#include "../common/strCompare.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	StrComp sc("concordia chiajna", "chiajna");
	auto res = sc.getStats();
	std::cout << res.identicalWordsNormalized << " " << res.relativeWordResemblance << "\n";
}

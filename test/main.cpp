/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "../common/strManip.h"
#include "../common/strCompare.h"
#include "../common/wordFreq.h"
#include "../common/log.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
	LOGPREFIX("test");

	std::ifstream f("../liste/fotbal");
	if (!f.is_open()) {
		ERROR("open file!");
		return -1;
	}
	std::vector<std::string> wordList;
	std::string line;
	while (std::getline(f, line)) {
		std::vector<std::string> words = strSplit(line, {' ', ';'});
		wordList.insert(wordList.end(), words.begin(), words.end());
	}
	WordFreqMap freqMap;
	freqMap.addWordList(wordList);

	std::cout << "idWords\tidWordsNorm\trelWordResemb\n";

	StrComp sc("asa", "asa targu mures");
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	auto res = sc.getStats();
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";

	sc = StrComp("asa", "astra giurgiu");
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	res = sc.getStats();
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";

	sc = StrComp("rapid bucuresti", "steaua bucuresti");
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	res = sc.getStats();
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";

	sc = StrComp("bucharest", "bucuresti");
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	res = sc.getStats();
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";

	sc = StrComp("rapid bucharest", "rapid bucuresti");
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	res = sc.getStats();
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.relativeWordResemblance << "\n\n";
}

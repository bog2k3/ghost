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
#include <set>

void compare(std::string const& s1, std::string const& s2, WordFreqMap const& freqMap) {
	StrComp sc(s1, s2);
	std::cout << sc.getS1() << " [vs] " << sc.getS2() << ":\n";
	auto res = sc.getStats(nullptr);
	std::cout << "[FM OFF]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.identicalWordsPercentage
			<< "\t" << res.relativeWordResemblance << "\t" << res.relativeLetterResemblance << "\n\n";
	res = sc.getStats(&freqMap);
	std::cout << "[FM ON ]" << res.identicalWords << "\t" << res.identicalWordsNormalized << "\t" << res.identicalWordsPercentage
			<< "\t" << res.relativeWordResemblance << "\t" << res.relativeLetterResemblance << "\n\n";
}

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
		std::set<std::string> uniqueWords;
		for (auto w : words) {
			uniqueWords.insert(w);
		}
		words.clear();
		words.assign(uniqueWords.begin(), uniqueWords.end());
		wordList.insert(wordList.end(), words.begin(), words.end());
	}
	std::vector<std::string> simpleWordList = {
			"asa", "bcd", "astra", "targu", "mures","giurgiu","rapid","bucuresti","steaua","dinamo","bucharest"
	}; // uniformFreq: f = 0.1
	WordFreqMap freqMap;
	freqMap.addWordList(wordList);
//	freqMap.addWordList(simpleWordList);


	std::cout << "idWords\tidWordsNorm\tidWordsPercent\trelWordResemb\trelLetterResmb\n";

	compare("asa", "asa", freqMap);
	compare("asa", "asa bcd", freqMap);
	compare("asa", "asa bcde", freqMap);
	compare("asa mures", "asa targu mures", freqMap);
	compare("asa", "asa targu mures", freqMap);
	compare("asa", "astra giurgiu", freqMap);
	compare("rapid bucuresti", "steaua bucuresti", freqMap);
	compare("bucharest", "bucuresti", freqMap);
	compare("rapid bucharest", "rapid bucuresti", freqMap);

//	freqMap.debugPrint(std::cout);
}

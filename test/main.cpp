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
#include <algorithm>

StrComp::Result compare(std::string const& s1, std::string const& s2, WordFreqMap const& freqMap) {
	StrComp sc(s1, s2);
	return sc.getStats(&freqMap);
}

void printRes(StrComp::Result const& res) {
#ifdef DEBUG
	std::cout << res.s1 << " [vs] " << res.s2 << ":\n";
#endif
	std::cout <<"idw:"<< res.identicalWords << "\tidwn:" << res.identicalWordsNormalized << "\tidw%:" << res.identicalWordsPercentage*100
			<< "\trwr:" << res.relativeWordResemblance << "\n\n";
}

int main(int argc, char* argv[]) {
	LOGPREFIX("test");

	std::ifstream f("../liste/fotbal");
	if (!f.is_open()) {
		ERROR("open file!");
		return -1;
	}
	std::vector<std::string> wordList;
	std::vector<std::string> echipe;
	std::string line;
	while (std::getline(f, line)) {
		std::vector<std::string> words = strSplit(line, {' ', ';'});
		auto echipeLinie = strSplit(line, ';');
		echipe.insert(echipe.end(), echipeLinie.begin(), echipeLinie.end());
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


	std::cout << "idWords\tidWordsNorm\tidWordsPercent\trelWordResemb\n\n";

	compare("caransebes", "rehden", freqMap);
//	compare("asa", "asa bcd", freqMap);
//	compare("asa", "asa bcde", freqMap);
//	compare("asa mures", "asa targu mures", freqMap);
//	compare("asa", "asa targu mures", freqMap);
//	compare("asa", "astra giurgiu", freqMap);
//	compare("rapid bucuresti", "steaua bucuresti", freqMap);
//	compare("bucharest", "bucuresti", freqMap);
//	compare("rapid bucharest", "rapid bucuresti", freqMap);

	std::srand(time(nullptr));
	std::vector<StrComp::Result> res;
	for (int i=0; i<1000; i++) {
		auto e1 = echipe[std::rand() * (echipe.size()-1) / RAND_MAX];
		auto e2 = echipe[std::rand() * (echipe.size()-1) / RAND_MAX];
		res.push_back(compare(e1, e2, freqMap));
	}
	std::sort(res.begin(), res.end(), [](StrComp::Result const&r1, StrComp::Result const& r2) {
		return r1.identicalWordsNormalized > r2.identicalWordsNormalized
				||
				(r1.identicalWordsNormalized == r2.identicalWordsNormalized &&
						r1.relativeWordResemblance > r2.relativeWordResemblance);
	});
	for (int i=0; i<100; i++)
		printRes(res[i]);


//	freqMap.debugPrint(std::cout);
}

/*
 PROBLEME:

dacia unirea braila [vs] dacia unirea braila:
idw:3	idwn:0.988235	idw%:100	rwr:1	rlr:1
asa targu mures [vs] asa targu mures:
idw:3	idwn:0.969231	idw%:100	rwr:1	rlr:1
asa targu mures [vs] asa targu mures:
idw:3	idwn:0.969231	idw%:100	rwr:1	rlr:1
farul constanta [vs] farul constanta:
idw:2	idwn:0.904762	idw%:100	rwr:1	rlr:1
universitatea craiova [vs] universitatea craiova:
idw:2	idwn:0.9	idw%:100	rwr:1	rlr:1
braila [vs] baia mare:
idw:1.06667	idwn:0.666667	idw%:53.3333	rwr:0.533333	rlr:0.666667

 */

/*
 * wordFreq.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: bog
 */

#include "wordFreq.h"
#include <ostream>

void WordFreqMap::addWordList(std::vector<std::string> words) {
	nTotalWords += words.size();
	for (auto w : words) {
		occurences[w]++;
	}
}

float WordFreqMap::getWordFreq(std::string const& word) const {
	auto it = occurences.find(word);
	return it == occurences.end() ? 0 : it->second / (float)nTotalWords;
}

void WordFreqMap::debugPrint(std::ostream &stream) {
	float div = 1.f / nTotalWords;
	for (auto it : occurences) {
		stream << it.first << "\t\t" << it.second * div << "\n";
	}
}

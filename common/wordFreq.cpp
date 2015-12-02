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

double WordFreqMap::getWordFreq(std::string const& word) const {
	auto it = occurences.find(word);
	return it == occurences.end() ? 0 : it->second / (double)nTotalWords;
}

void WordFreqMap::debugPrint(std::ostream &stream) {
	double div = 1.0 / nTotalWords;
	for (auto it : occurences) {
		stream << it.first << "\t\t" << it.second * div << "\n";
	}
}

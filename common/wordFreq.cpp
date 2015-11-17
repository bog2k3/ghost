/*
 * wordFreq.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: bog
 */

#include "wordFreq.h"

void WordFreqMap::addWordList(std::vector<std::string> words) {
	nTotalWords += words.size();
	for (auto w : words) {
		occurences[w]++;
	}
}

float WordFreqMap::getWordFreq(std::string const& word) {
	return occurences[word] / (float)nTotalWords;
}

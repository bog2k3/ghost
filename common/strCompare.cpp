/*
 * strCompare.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#include "strCompare.h"
#include "strManip.h"
#include "wordFreq.h"
#include <algorithm>
#include <map>

StrComp::StrComp(std::string const& s1, std::string const& s2)
	: s1(s1), s2(s2)
{
	preprocess();
}

void StrComp::preprocess() {
	// place string with fewer words on first position
	if (std::count(s1.begin(), s1.end(), ' ') > std::count(s2.begin(), s2.end(), ' ')) {
		std::string saux = s1;
		s1 = s2;
		s2 = saux;
	}

	// 6. split to words:
	s1w = strSplit(s1, ' ');
	s2w = strSplit(s2, ' ');
}

int StrComp::getAbsLetterDiff(std::string const& t1, std::string const& t2) {
	struct letterInfo {
		int n1 = 0;	// how many times it's in s1
		int n2 = 0;	// how many times it's in s2
	};
	std::map<wchar_t, letterInfo> mapLetters;
	for (wchar_t c : t1)
		mapLetters[c].n1++;
	for (wchar_t c : t2)
		mapLetters[c].n2++;
	int diff = 0;
	for (auto &pair : mapLetters)
		diff += abs(pair.second.n1 - pair.second.n2);
	return diff;
}

StrComp::Result StrComp::getStats(WordFreqMap* pFreqMap) {
	StrComp::Result res(this);
	if (s1w.empty() || s2w.empty())
		return res;

	// 1. relative letter resemblance
	size_t lenTotal = s1.length() + s2.length();
	size_t maxLenNoSp = std::max(s1.length() - std::count(s1.begin(), s1.end(), ' '),
								s2.length() - std::count(s2.begin(), s2.end(), ' '));
	res.relativeLetterResemblance = 1.f - (float)getAbsLetterDiff(s1, s2) / lenTotal;
	int totalWords = (s1w.size() + s2w.size());
	// 2. average number of words:
	res.totalWordsAvg = totalWords / 2.f;
	// 3. if we have a word freq map, we compute the total weight of all words
	float maxWeight = 0;
	if (pFreqMap) {
		float maxW1 = 0;
		for (auto w : s1w)
			maxW1 += pFreqMap->getWordFreq(w);
		float maxW2 = 0;
		for (auto w : s2w)
			maxW2 += pFreqMap->getWordFreq(w);
		maxWeight = std::max(maxW1, maxW2);
	} else
		maxWeight = std::max(s1w.size(), s2w.size());
	// 4. identical & similar words:
	res.identicalWords = 0;
	res.relativeWordResemblance = 0;
	res.identicalWordsNormalized = 0;
	for (unsigned i=0; i<s1w.size(); i++) {
		float maxResemblance = 0;
		for (unsigned j=0; j<s2w.size(); j++) {
			float similarity = 1.f - (float)getAbsLetterDiff(s1w[i], s2w[j]) / (s1w[i].length() + s2w[j].length());
			if (similarity > maxResemblance) {
				maxResemblance = similarity;
			}
			if (maxResemblance == 1)
				break;
		}

		float wordWeight = pFreqMap ? pFreqMap->getWordFreq(s1w[i]) / maxWeight : 1;

		res.relativeWordResemblance += maxResemblance * wordWeight;
		if (maxResemblance > 0.85 && maxResemblance > 1 - 2.f / s1w[i].length()) {	// words differ by at most 2 letters
			res.identicalWords += maxResemblance * wordWeight;
			res.identicalWordsNormalized += maxResemblance * (float)s1w[i].length() / maxLenNoSp * wordWeight;
		}
	}
	res.relativeWordResemblance /= maxWeight;// (float)std::max(s1w.size(), s2w.size());
	res.identicalWordsPercentage = res.identicalWords / maxWeight; //res.totalWordsAvg;

	return res;
}

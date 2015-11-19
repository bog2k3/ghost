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

StrComp::Result StrComp::getStats(const WordFreqMap* pFreqMap) {
	StrComp::Result res(this);
	if (s1w.empty() || s2w.empty())
		return res;

	// 1. relative letter resemblance
	size_t lenTotal = s1.length() + s2.length();
	size_t maxLenNoSp = std::max(s1.length() - std::count(s1.begin(), s1.end(), ' '),
								s2.length() - std::count(s2.begin(), s2.end(), ' '));
	res.relativeLetterResemblance = 1.f - (float)getAbsLetterDiff(s1, s2) / lenTotal;
	int maxWords = std::max(s1w.size(), s2w.size());
	// 2. average number of words:
	res.totalWordsAvg = (s1w.size() + s2w.size()) / 2.f;
	float uniformWeight1 = 1.f / s1w.size(); // this is the uniform weight for each word if we don't have a freq map
	float uniformWeight2 = 1.f / s2w.size();
	// 3. if we have a word freq map, we compute the total weight of all words
	std::map<std::string, float> wordWeights1;
	std::map<std::string, float> wordWeights2;
	if (pFreqMap) {
		/*
		 * formula is :
		 * 	wordWeight = k / wordFreq
		 * 	where
		 * 		k is a constant computed below
		 * 		wordFreq is the frequency of the word in our lists
		 *
		 * 	k = 1 / sum(i, 1/wordFreq[i]), i iterates through all the words in each string (s1w and s2w)
		 * 	this ensures that the sum of all word weights for each string equals 1.0
		 */
		float freqK1 = 0.f; // this is a constant used in computing the weights of each word from their frequencies related to the other words in the strings
		float freqK2 = 0.f;
		// the sum of the word weights for all words in each s1 and s2 must equal 1.0
		for (auto w : s1w) {
			float wFreq = pFreqMap->getWordFreq(w);
			if (wFreq == 0)
				wFreq = 1.f / pFreqMap->getSampleSize();
			wordWeights1[w] = wFreq;
			freqK1 += 1.f / wFreq;
		}
		for (auto w : s2w) {
			float wFreq = pFreqMap->getWordFreq(w);
			if (wFreq == 0)
				wFreq = 1.f / pFreqMap->getSampleSize();
			wordWeights2[w] = wFreq;
			freqK2 += 1.f / wFreq;
		}

		freqK1 = 1.f / freqK1;
		freqK2 = 1.f / freqK2;

		for (auto w : s1w) {
			wordWeights1[w] = freqK1 / wordWeights1[w];
		}
		for (auto w : s2w) {
			wordWeights2[w] = freqK2 / wordWeights2[w];
		}
	}
	// 4. identical & similar words:
	res.identicalWords = 0;
	res.relativeWordResemblance = 0;
	res.identicalWordsNormalized = 0;
	for (unsigned i=0; i<s1w.size(); i++) {
		float maxResemblance = 0;
		int secondIndex = -1;
		for (unsigned j=0; j<s2w.size(); j++) {
			float similarity = 1.f - (float)getAbsLetterDiff(s1w[i], s2w[j]) / (s1w[i].length() + s2w[j].length());
			if (similarity > maxResemblance) {
				maxResemblance = similarity;
				secondIndex = j;
			}
			if (maxResemblance == 1)
				break;
		}

		float w1Weight = pFreqMap ? wordWeights1[s1w[i]] : uniformWeight1;
		float w2Weight = (pFreqMap && secondIndex != -1) ? wordWeights2[s2w[secondIndex]] : uniformWeight2;
		float crtMinWeight = std::min(w1Weight, w2Weight);
		float crtUnifWeight = crtMinWeight == w1Weight ? uniformWeight1 : uniformWeight2;

		res.relativeWordResemblance += maxResemblance * crtMinWeight;
		if (maxResemblance > 0.75 && maxResemblance > 1 - 2.f / s1w[i].length()) {	// words differ by at most 2 letters
			float idWordsDelta = maxResemblance * crtMinWeight / crtUnifWeight;
			res.identicalWords += idWordsDelta;
			res.identicalWordsPercentage += idWordsDelta / maxWords;
			res.identicalWordsNormalized += idWordsDelta * (s1w[i].length() + s2w[secondIndex].length())*0.5f / maxLenNoSp;
		}
	}

	return res;
}

/*
 * strCompare.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#include "strCompare.h"
#include "strManip.h"
#include "wordFreq.h"
#include "sanitize.h"
#include "assert.h"
#include <algorithm>
#include <map>

#ifdef DEBUG
#define assertSanitized(str) {\
	std::string s1(str.c_str());\
	sanitize(str);\
	assertDbg(s1 == str && "Bă, sanitizeaza stringurile inainte sa le pasezi la StrComp!!!");\
}
#else
#define assertSanitized(str)
#endif

StrComp::StrComp(std::string const& s1, std::string const& s2)
	: s1_(s1), s2_(s2)
{
	assertSanitized(s1_);
	assertSanitized(s2_);
	preprocess();
}

void StrComp::preprocess() {
	// place string with fewer words on first position (or if same number of words, the shorter one)
	auto nw1 = std::count(s1_.begin(), s1_.end(), ' ');
	auto nw2 = std::count(s2_.begin(), s2_.end(), ' ');
	if (nw1 > nw2 || (nw1 == nw2 && s1_.length() > s2_.length())) {
		std::string saux = s1_;
		s1_ = s2_;
		s2_ = saux;
	}

	// 6. split to words:
	s1w_ = strSplit(s1_, ' ');
	s2w_ = strSplit(s2_, ' ');
}

int StrComp::getAbsLetterDiff(std::string const& t1, std::string const& t2) {
	struct letterInfo {
		int n1 = 0;	// how many times it's in s1
		int n2 = 0;	// how many times it's in s2
	};
	std::map<char, letterInfo> mapLetters;
	for (char c : t1)
		mapLetters[c].n1++;
	for (char c : t2)
		mapLetters[c].n2++;
	int diff = 0;
	for (auto &pair : mapLetters)
		diff += abs(pair.second.n1 - pair.second.n2);
	return diff;
}

std::vector<std::string> StrComp::generateNGrams(int n, std::string const& s) {
	std::vector<std::string> ngrams;
	if (n > s.size()) {
		ngrams.push_back(s);
	} else
		for (size_t i=0; i<=s.size()-n; i++)
			ngrams.push_back(s.substr(i, n));
	return ngrams;
}

void StrComp::getNGramResemblance(int n, std::string const& s1, std::string const& s2, double &out_similarity, int &out_longestStreak) {
	std::vector<std::string> ngrams1 = generateNGrams(n, s1);
	std::vector<std::string> ngrams2 = generateNGrams(n, s2);
	if (s1.length() > s2.length())	// make sure we have the shortest string on the first position
		ngrams1.swap(ngrams2);
	out_longestStreak = 0;
	out_similarity = 0;

	std::vector<std::pair<int, int>> searchPos;	// holds search positions for the current ngram. first is the next position to check, second is the current streak

	for (unsigned i=0; i<ngrams1.size(); i++) {
		if (searchPos.empty())
			searchPos.push_back(std::make_pair(0, 0));
		decltype(searchPos) nextSearchPos;
		// search for the current ngram in the second string from all current positions
		bool foundMatch = false;
		for (unsigned j=0; j<searchPos.size(); j++) {
			unsigned stop = searchPos[j].first;	// this is the position where we stop the current search
			if (searchPos.size() > 1)
				stop = searchPos[(j+1)%searchPos.size()].first;
			bool first_compare = true;
			for (unsigned k=searchPos[j].first; first_compare || k!=stop; k = (k+1)%ngrams2.size()) {
				first_compare = false;
				if (ngrams1[i] == ngrams2[k]) {
					// found a match
					foundMatch = true;
					if (k < ngrams2.size()-1) // reached the end?
						nextSearchPos.push_back(std::make_pair(k+1, searchPos[j].second+1));
					if (out_longestStreak < searchPos[j].second+1)
						out_longestStreak = searchPos[j].second+1;
				}
			}
		}
		searchPos.swap(nextSearchPos);
		if (foundMatch)
			out_similarity++;  // at least one match was found for this ngram
	}
	out_similarity /= ngrams2.size();
}

StrComp::Result StrComp::getStats(const WordFreqMap* pFreqMap) {
	StrComp::Result res;
	res.s1 = s1_;
	res.s2 = s2_;
	if (s1w_.empty() || s2w_.empty())
		return res;

	// 1. relative letter resemblance
	size_t lenTotal = s1_.length() + s2_.length();
	size_t maxLenNoSp = std::max(s1_.length() - std::count(s1_.begin(), s1_.end(), ' '),
								s2_.length() - std::count(s2_.begin(), s2_.end(), ' '));
	res.relativeLetterResemblance = 1.0 - (double)getAbsLetterDiff(s1_, s2_) / lenTotal;
	int maxWords = std::max(s1w_.size(), s2w_.size());
	// 2. average number of words:
	res.totalWordsAvg = (s1w_.size() + s2w_.size()) / 2.0;
	double uniformWeight1 = 1.0 / s1w_.size(); // this is the uniform weight for each word if we don't have a freq map
	double uniformWeight2 = 1.0 / s2w_.size();
	// 3. if we have a word freq map, we compute the total weight of all words
	std::map<std::string, double> wordWeights1;
	std::map<std::string, double> wordWeights2;
	if (pFreqMap) {
		/*
		 * formula is :
		 * 	wordWeight = k / wordFreq
		 * 	where
		 * 		k is a constant computed below
		 * 		wordFreq is the frequency of the word in our lists
		 *
		 * 	k = 1 / sum(i, 1/wordFreq[i]), i iterates through all the words in each string (s1w_ and s2w_)
		 * 	this ensures that the sum of all word weights for each string equals 1.0
		 */
		double freqK1 = 0.0; // this is a constant used in computing the weights of each word from their frequencies related to the other words in the strings
		double freqK2 = 0.0;
		// the sum of the word weights for all words in each s1 and s2 must equal 1.0
		for (auto w : s1w_) {
			double wFreq = pFreqMap->getWordFreq(w);
			if (wFreq == 0)
				wFreq = 1.0 / pFreqMap->getSampleSize();
			wordWeights1[w] = wFreq;
			freqK1 += 1.0 / wFreq;
		}
		for (auto w : s2w_) {
			double wFreq = pFreqMap->getWordFreq(w);
			if (wFreq == 0)
				wFreq = 1.0 / pFreqMap->getSampleSize();
			wordWeights2[w] = wFreq;
			freqK2 += 1.0 / wFreq;
		}

		freqK1 = 1.0 / freqK1;
		freqK2 = 1.0 / freqK2;

		for (auto w : s1w_) {
			wordWeights1[w] = freqK1 / wordWeights1[w];
		}
		for (auto w : s2w_) {
			wordWeights2[w] = freqK2 / wordWeights2[w];
		}
	}
	// 4. identical & similar words:
	res.identicalWords = 0;
	res.relativeWordResemblance = 0;
	res.identicalWordsNormalized = 0;
	for (unsigned i=0; i<s1w_.size(); i++) {
		double maxResemblance = 0.0;
		int secondIndex = -1;
		for (unsigned j=0; j<s2w_.size(); j++) {
			double similarity;
			int longestStreak;
			getNGramResemblance(2, s1w_[i], s2w_[j], similarity, longestStreak);
			// longer streaks mean better resemblance:
			int maxStreak = std::max(std::min(s1w_[i].length(), s2w_[j].length()) - 1, (size_t)1);
			similarity *= (double)longestStreak / maxStreak;
			if (similarity > maxResemblance) {
				maxResemblance = similarity;
				secondIndex = j;
			}
			if (maxResemblance == 1)
				break;
		}

		double w1Weight = pFreqMap ? wordWeights1[s1w_[i]] : uniformWeight1;
		double w2Weight = (pFreqMap && secondIndex != -1) ? wordWeights2[s2w_[secondIndex]] : uniformWeight2;
		double crtMinWeight = std::min(w1Weight, w2Weight);
		double crtUnifWeight = crtMinWeight == w1Weight ? uniformWeight1 : uniformWeight2;

		res.relativeWordResemblance += maxResemblance * crtMinWeight;
		if (maxResemblance > 0.75 && maxResemblance > 1 - 2.0 / s1w_[i].length()) {	// words differ by at most 2 letters
			double idWordsDelta = maxResemblance * crtMinWeight / crtUnifWeight;
			res.identicalWords += idWordsDelta;
			res.identicalWordsPercentage += idWordsDelta / maxWords;
			res.identicalWordsNormalized += idWordsDelta * (s1w_[i].length() + s2w_[secondIndex].length())*0.5 / maxLenNoSp;
		}
	}

	return res;
}

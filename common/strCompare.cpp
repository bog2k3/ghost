/*
 * strCompare.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#include "strCompare.h"
#include "strManip.h"
#include <algorithm>
#include <map>

StrComp::StrComp(std::string const& s1, std::string const& s2, invalidCharHandler handler)
	: s1(s1), s2(s2)
{
	preprocess(handler);
}

void StrComp::preprocess(invalidCharHandler handler) {
	// 1. strip '.'
	s1.erase(std::remove(s1.begin(), s1.end(), '.'), s1.end());
	s2.erase(std::remove(s2.begin(), s2.end(), '.'), s2.end());
	// 2. replace '-' with ' '
	std::replace(s1.begin(), s1.end(), '-', ' ');
	std::replace(s2.begin(), s2.end(), '-', ' ');
	// 3. place string with fewer words on first position
	if (std::count(s1.begin(), s1.end(), ' ') > std::count(s2.begin(), s2.end(), ' ')) {
		std::string saux = s1;
		s1 = s2;
		s2 = saux;
	}
	// 4. tolower:
	strLower(s1);
	strLower(s2);

	// 5. replace diacritics:
	removeDiacritics(s1, handler);
	removeDiacritics(s2, handler);

	// 6. split to words:
	s1w = strSplit(s1, L' ');
	s2w = strSplit(s2, L' ');
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

StrComp::Result StrComp::getStats() {
	StrComp::Result res;

	// 1. relative letter resemblance
	size_t lenTotal = s1.length() + s2.length();
	size_t lenTotalNoSp = lenTotal - std::count(s1.begin(), s1.end(), ' ')
									- std::count(s2.begin(), s2.end(), ' ');
	res.relativeLetterResemblance = 1.f - (float)getAbsLetterDiff(s1, s2) / lenTotal;
	// 2. average number of words:
	res.totalWordsAvg = (s1w.size() + s2w.size()) / 2.f;
	// 3. identical words:
	res.identicalWords = 0;
	res.relativeWordResemblance = 0;
	res.identicalWordsNormalized = 0;
	for (unsigned i=0; i<s1w.size(); i++) {
		float maxResemblance = 0;
		for (unsigned j=0; j<s2w.size(); j++) {
			float res = 1.f - (float)getAbsLetterDiff(s1w[i], s2w[j]) / (s1w[i].length() + s2w[j].length());
			if (res > maxResemblance) {
				maxResemblance = res;
			}
			if (maxResemblance == 1)
				break;
		}

		res.relativeWordResemblance += maxResemblance;
		if (maxResemblance == 1) {
			res.identicalWords++;
			res.identicalWordsNormalized += s1w[i].length() * 2.f / lenTotalNoSp;
		}
	}
	res.relativeWordResemblance /= res.totalWordsAvg;
	res.identicalWordsPercentage = res.identicalWords / res.totalWordsAvg;

	return res;
}

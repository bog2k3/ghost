/*
 * strCompare.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#include "strCompare.h"
#include "wstrManip.h"

#include <algorithm>
#include <map>

// TODO - de citit dintr-un fisier de config:
std::map<std::wstring, std::wstring> mapDiacrit = {
		{L"ă", L"a"},
		{L"â", L"a"},
		{L"î", L"i"},
		{L"ș", L"s"},
		{L"ț", L"t"},
};

StrComp::StrComp(std::wstring const& s1, std::wstring const& s2)
	: s1(s1), s2(s2)
{
	preprocess();
}

void StrComp::replaceSubstr(std::wstring &str, std::wstring const& what, std::wstring const& replacement) {
	size_t pos = 0;
	while ((pos = str.find(what, pos)) != str.npos) {
		str.replace(str.begin()+pos, str.begin()+pos+what.length(), replacement);
		pos += replacement.length();
	}
}

void StrComp::preprocess() {
	// 1. strip '.'
	s1.erase(std::remove(s1.begin(), s1.end(), '.'), s1.end());
	s2.erase(std::remove(s2.begin(), s2.end(), '.'), s2.end());
	// 2. replace '-' with ' '
	std::replace(s1.begin(), s1.end(), '-', ' ');
	std::replace(s2.begin(), s2.end(), '-', ' ');
	// 3. place string with fewer words on first position
	if (std::count(s1.begin(), s1.end(), ' ') > std::count(s2.begin(), s2.end(), ' ')) {
		std::wstring saux = s1;
		s1 = s2;
		s2 = saux;
	}
	// 4. tolower:
	wToLower(s1);
	wToLower(s2);

	// 5. replace diacritics:
	for (auto &pair : mapDiacrit) {
		replaceSubstr(s1, pair.first, pair.second);
		replaceSubstr(s2, pair.first, pair.second);
	}
	// 6. split to words:
	s1w = wstrSplit(s1, L' ');
	s2w = wstrSplit(s2, L' ');
}

int StrComp::getAbsLetterDiff(std::wstring const& t1, std::wstring const& t2) {
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

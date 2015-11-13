/*
 * strCompare.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#include "strCompare.h"
#include <algorithm>
#include <sstream>
#include <locale>
#include <map>

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
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<wchar_t> >(loc);
	f.tolower(const_cast<wchar_t*>(s1.c_str()), s1.c_str()+s1.length());
	f.tolower(const_cast<wchar_t*>(s2.c_str()), s2.c_str()+s2.length());
	// 5. replace diacritics:
	replaceSubstr(s1, L"ă", L"a");
	replaceSubstr(s1, L"â", L"a");
	replaceSubstr(s1, L"î", L"i");
	replaceSubstr(s1, L"ș", L"s");
	replaceSubstr(s1, L"ț", L"t");

	replaceSubstr(s2, L"ă", L"a");
	replaceSubstr(s2, L"â", L"a");
	replaceSubstr(s2, L"î", L"i");
	replaceSubstr(s2, L"ș", L"s");
	replaceSubstr(s2, L"ț", L"t");

	// 6. split to words:
	std::wstring tk;
	std::wstringstream ss1(s1);
	while (!ss1.eof()) {
		ss1 >> tk;
		s1w.push_back(tk);
	}
	std::wstringstream ss2(s2);
	while (!ss2.eof()) {
		ss2 >> tk;
		s2w.push_back(tk);
	}
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

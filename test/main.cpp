/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <sstream>
#include <locale>
#include <map>

class StrComp {
public:

	struct Result {
		float relativeLetterResemblance;	// how many identical letters relative to the total number of letters
		float totalWordsAvg;				// average of total words from s1 and s2
		float relativeWordResemblance;		// percentage that words in the two texts match 1:1
		int identicalWords;					// how many identical words
		float identicalWordsPercentage;		// percentage of identical words between s1 and s2
		float identicalWordsNormalized;		// number of identical words normalized by their length relative to the text's average length
	};

	StrComp(std::wstring const& s1, std::wstring const& s2);
	Result getStats();

private:
	void preprocess();
	void replaceSubstr(std::wstring &str, std::wstring const& what, std::wstring const& replacement);
	int getAbsLetterDiff(std::wstring const& s1, std::wstring const& s2);
	std::wstring s1, s2;
	std::vector<std::wstring> s1w, s2w;	// words
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
		int maxIndex = -1;
		for (unsigned j=0; j<s2w.size(); j++) {
			float res = 1.f - (float)getAbsLetterDiff(s1w[i], s2w[j]) / (s1w[i].length() + s2w[j].length());
			if (res > maxResemblance) {
				maxResemblance = res;
				maxIndex = j;
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

int main(int argc, char* argv[]) {
	const wchar_t* nume[] = {
		L"ASA Tg Mures",		// 0
		L"ASA Targu Mures",		// 1
		L"ASA Târgu Mureș",		// 2
		L"Astra Giurgiu",		// 3
		L"Astra G",				// 4
		L"Bucharest",			// 5
		L"București",			// 6
		L"Bucuresti"			// 7
	};

	StrComp::Result res[10][10];
	for (int i=0; i<sizeof(nume)/sizeof(nume[0]); i++)
		for (int j=0; j<sizeof(nume)/sizeof(nume[0]); j++) {
			StrComp scomp(nume[i], nume[j]);
			res[i][j] = scomp.getStats();
		}

	return 0;
}

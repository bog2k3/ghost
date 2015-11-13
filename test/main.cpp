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

class StrComp {
public:

	struct Result {
		float relativeLetterResemblance;	// how many identical letters relative to the total number of letters
		int identicalWords;					// how many identical words
		float totalWordsAvg;				// average of total words from s1 and s2
		float identicalWordsPercentage;		// percentage of identical words between s1 and s2
		float identicalWordsNormalized;		// number of identical words normalized by their length relative to the text's average length
	};

	StrComp(std::wstring const& s1, std::wstring const& s2);
	Result getStats();

private:
	void preprocess();
	void replaceSubstr(std::wstring &str, std::wstring const& what, std::wstring const& replacement);
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
	// 3. tolower:
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<wchar_t> >(loc);
	f.tolower(const_cast<wchar_t*>(s1.c_str()), s1.c_str()+s1.length());
	f.tolower(const_cast<wchar_t*>(s2.c_str()), s2.c_str()+s2.length());
	// 4. replace diacritics:
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

	// split to words:
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

StrComp::Result StrComp::getStats() {
	StrComp::Result res;

	// 1. relative letter resemblance
	size_t lenTotal = s1.length() + s2.length();

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

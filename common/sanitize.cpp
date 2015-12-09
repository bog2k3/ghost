/*
 * sanitize.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: bog
 */

#include "sanitize.h"
#include "strManip.h"
#include "assert.h"
#include <map>
#include <locale>
#include <algorithm>

std::map<std::string, std::string> mapDiacrit = {
		{"ă", "a"}, {"Ă", "A"},
		{"â", "a"}, {"Â", "A"},
		{"î", "i"}, {"Î", "I"},
		{"ș", "s"}, {"Ș", "S"},
		{"ț", "t"}, {"Ț", "T"},
};

bool removeDiacritics(std::string &in_out) {
	for (auto &pair : mapDiacrit)
		replaceAllSubstr(in_out, pair.first, pair.second);
	for (char c : in_out)
		if (c & 0x80)
			return false; // invalid ASCII multi-byte char detected - that means we don't have it in our map
	return true;
}

sanitizeResult sanitize(std::string &s) {
	sanitizeResult res;
	// 1a. strip '.'
	s.erase(std::remove(s.begin(), s.end(), '.'), s.end());

	// 2. replace '-' with ' '
	std::replace(s.begin(), s.end(), '-', ' ');

	// 3. replace diacritics:
	if (!removeDiacritics(s))
		res.failedDiacritics = true;

	// 4. tolower:
	if (!res.failedDiacritics)
		s = strLower(s);

	// 5. remove age restrictions:
	size_t pos = 0;
	if ((pos = s.find("u19")) != s.npos) {
		res.age_u19 = true;
		s = s.substr(0, pos) + s.substr(pos+3);
	}
	if ((pos = s.find("u 19")) != s.npos) {
		res.age_u19 = true;
		s = s.substr(0, pos) + s.substr(pos+4);
	}
	if ((pos = s.find("u20")) != s.npos) {
		res.age_u20 = true;
		s = s.substr(0, pos) + s.substr(pos+3);
	}
	if ((pos = s.find("u 20")) != s.npos) {
		res.age_u20 = true;
		s = s.substr(0, pos) + s.substr(pos+4);
	}
	if ((pos = s.find("u21")) != s.npos) {
		res.age_u21 = true;
		s = s.substr(0, pos) + s.substr(pos+3);
	}
	if ((pos = s.find("u 21")) != s.npos) {
		res.age_u21 = true;
		s = s.substr(0, pos) + s.substr(pos+4);
	}
	if ((pos = s.find("(t)")) != s.npos) {
		res.tineret = true;
		s = s.substr(0, pos) + s.substr(pos+3);
	}
	if ((pos = s.find("(f)")) != s.npos) {
		res.feminin = true;
		s = s.substr(0, pos) + s.substr(pos+3);
	}

	// 6. trim trailing spaces
	while (s.back() == ' ')
		s.pop_back();

	return res;
}

std::vector<sanitizeResult> sanitize(std::vector<std::string> &vs) {
	std::vector<sanitizeResult> res;
	for (auto &s : vs)
		res.push_back(sanitize(s));
	return res;
}

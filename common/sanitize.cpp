/*
 * sanitize.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: bog
 */

#include "sanitize.h"
#include "strManip.h"
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
	// 1. strip '.'
	s.erase(std::remove(s.begin(), s.end(), '.'), s.end());

	// 2. replace '-' with ' '
	std::replace(s.begin(), s.end(), '-', ' ');

	// 3. replace diacritics:
	if (!removeDiacritics(s))
		res.failedDiacritics = true;

	// 4. tolower:
	if (!res.failedDiacritics)
		strLower(s);

	// 5. remove age restrictions:
	size_t u19pos = 0, u21pos = 0;
	if ((u19pos = s.find("u19")) != s.npos) {
		res.age_u19 = true;
		s = s.substr(0, u19pos) + s.substr(u19pos+3);
	}
	if ((u19pos = s.find("u 19")) != s.npos) {
		res.age_u19 = true;
		s = s.substr(0, u19pos) + s.substr(u19pos+4);
	}
	if ((u21pos = s.find("u21")) != s.npos) {
		res.age_u21 = true;
		s = s.substr(0, u21pos) + s.substr(u21pos+3);
	}
	if ((u21pos = s.find("u 21")) != s.npos) {
		res.age_u21 = true;
		s = s.substr(0, u21pos) + s.substr(u21pos+4);
	}

	return res;
}

std::vector<sanitizeResult> sanitize(std::vector<std::string> &vs) {
	std::vector<sanitizeResult> res;
	for (auto &s : vs)
		res.push_back(sanitize(s));
	return res;
}

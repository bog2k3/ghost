/*
 * strManip.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "strManip.h"
#include <algorithm>
#include <map>
#include <locale>

// TODO - de citit dintr-un fisier de config:
std::map<std::string, std::string> mapDiacrit = {
		{"ă", "a"}, {"Ă", "A"},
		{"â", "a"}, {"Â", "A"},
		{"î", "i"}, {"Î", "I"},
		{"ș", "s"}, {"Ș", "S"},
		{"ț", "t"}, {"Ț", "T"},
};

std::vector<std::string> strSplit(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != text.npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

void strLower(std::string &in_out) {
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<char> >(loc);
	f.tolower(const_cast<char*>(in_out.c_str()), in_out.c_str() + in_out.length());
}

void replaceAllSubstr(std::string &str, std::string const& what, std::string const& replacement) {
	size_t pos = 0;
	while ((pos = str.find(what, pos)) != str.npos) {
		str.replace(str.begin()+pos, str.begin()+pos+what.length(), replacement);
		pos += replacement.length();
	}
}

void removeDiacritics(std::string &in_out) {
	for (auto &pair : mapDiacrit) {
		replaceAllSubstr(in_out, pair.first, pair.second);
	}
}

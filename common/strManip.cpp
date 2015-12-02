/*
 * strManip.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "strManip.h"
#include <algorithm>
#include <locale>

std::vector<std::string> strSplit(std::string const& text, char sep) {
	return strSplit(text, std::vector<char>{sep});
}

std::vector<std::string> strSplit(std::string const& text, std::vector<char> const& sep) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	do {
		end = text.npos;
		for (auto c : sep) {
			auto end1 = text.find(c, start);
			if (end1 < end)
				end = end1;
		}
		if (end != text.npos) {
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
	} while (end != text.npos);

	tokens.push_back(text.substr(start));
	return tokens;
}

void strLower(std::string &in_out) {
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<char> >(loc);
	f.tolower(const_cast<char*>(in_out.c_str()), in_out.c_str() + in_out.length());
}

void strUpper(std::string &in_out) {
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<char> >(loc);
	f.toupper(const_cast<char*>(in_out.c_str()), in_out.c_str() + in_out.length());
}

void replaceAllSubstr(std::string &str, std::string const& what, std::string const& replacement) {
	size_t pos = 0;
	while ((pos = str.find(what, pos)) != str.npos) {
		str.replace(str.begin()+pos, str.begin()+pos+what.length(), replacement);
		pos += replacement.length();
	}
}

unsigned getNumberOfWords(std::string const& str) {
	return std::count(str.begin(), str.end(), ' ');
}

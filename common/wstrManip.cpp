/*
 * wstrManip.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "wstrManip.h"
#include <locale>

std::vector<std::wstring> strSplit(const std::wstring &text, wchar_t sep) {
	std::vector<std::wstring> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::wstring::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

void wToLower(std::wstring &ws_in_out) {
	std::locale loc("en_US.UTF8");
	auto &f = std::use_facet<std::ctype<wchar_t> >(loc);
	f.tolower(const_cast<wchar_t*>(ws_in_out.c_str()), ws_in_out.c_str() + ws_in_out.length());
}

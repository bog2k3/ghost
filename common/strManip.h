/*
 * strManip.h
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include <string>
#include <vector>

std::vector<std::string> strSplit(const std::string &text, char sep);
void strLower(std::string &in_out);
void replaceAllSubstr(std::string &str, std::string const& what, std::string const& replacement);
void removeDiacritics(std::string &in_out);

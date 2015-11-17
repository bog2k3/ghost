/*
 * strManip.h
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include <string>
#include <vector>

unsigned getNumberOfWords(std::string const& str);
std::vector<std::string> strSplit(std::string const& text, char sep);
void strLower(std::string &in_out);
void replaceAllSubstr(std::string &str, std::string const& what, std::string const& replacement);

/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include "../common/strManip.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	std::string str(argv[1]);
	std::cout << "argv[1]: " << argv[1] << "\n";
	std::cout << "str.len: " << str.length() << "\n";
	removeDiacritics(str);
	strLower(str);
	std::cout << "str conv: " << str << "\n";
}

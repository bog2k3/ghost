/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: bog
 */

#include <iostream>
#include <string>
#include <locale>
#include <sstream>

int main(int argc, char* argv[]) {

	std::wstringstream ss;
	std::locale loc("en_US.UTF8");
//	ss.imbue(loc);
//	std::wcout.imbue(loc);

	ss << argv[1];
	std::wstring str;
	ss >> str;

	std::wcout << "argv[1]: " << argv[1] << "\n";
	std::wcout << "wstr: " << str << "\n";

	return 0;
}

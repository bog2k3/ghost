/*
 * configFile.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: bog
 */

#include "configFile.h"
#include "log.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

bool parseConfigFile(std::string const& filePath, std::map<std::string, std::string> &opts, std::vector<std::string> const& requiredOpts) {
	std::ifstream f(filePath);
	if (!f.is_open())
		return false;

	LOGLN("citim fisierul de config \"" << filePath << "\"...");

	std::string line;
	try {
		while (std::getline(f, line)) {
			if (line.empty())
				continue;

			std::stringstream ss(line);

			std::string tokenName, equalSign, value;
			ss >> tokenName >> equalSign >> value;

			if (equalSign != "=") {
				ERROR("Linie invalida in fisierul de config:\n"<<line);
			}

			opts[tokenName] = value;
		}
		LOGLN("am terminat de parsat fisierul de config.");
	} catch (std::runtime_error &err) {
		ERROR("Citirea fisierului de config \"" << filePath <<"\"\n" << err.what());
	}

	for (auto o : requiredOpts) {
		if (opts.find(o) == opts.end()) {
			ERROR("Lipseste optiunea \"" << o << "\" din fisierul de config !!!");
			return false;
		}
	}
	return true;
}



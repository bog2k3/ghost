/*
 * configFile.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: bog
 */

#include "cmdLine.h"
#include "../cpplib/log.h"
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>

void parseConfigFile(cmdLineOptions &opts) {
	std::ifstream f(opts.configFilePath);
	if (!f.is_open())
		return;

	LOGLN("citim fisierul de config \"" << opts.configFilePath << "\"...");

	std::map<std::string, std::string*> mapKeyValues;
	mapKeyValues["URI"] = &opts.dbURI;
	mapKeyValues["user"] = &opts.dbUser;
	mapKeyValues["passw"] = &opts.dbPassw;
	mapKeyValues["database"] = &opts.dbName;

	std::string line;
	try {
		while (std::getline(f, line)) {
			if (line.empty())
				continue;

			std::stringstream ss(line);

			std::string tokenName, equalSign, value;
			ss >> tokenName >> equalSign >> value;

			if (mapKeyValues.find(tokenName) == mapKeyValues.end()) {
				ERROR("Cheie necunoscuta \"" << tokenName << "\" in fisierul de config!");
				continue;
			}

			*mapKeyValues[tokenName] = value;
		}
		LOGLN("am terminat de parsat fisierul de config.");
	} catch (std::runtime_error &err) {
		ERROR("Citirea fisierului de config \"" << opts.configFilePath <<"\"\n" << err.what());
	}
}



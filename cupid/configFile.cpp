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

	LOG("parsing config file \"" << opts.configFilePath << "\"...");

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
				ERROR("Unknown key \"" << tokenName << "\" in config file");
				continue;
			}

			*mapKeyValues[tokenName] = value;
		}
		LOG("finished parsing config file.");
	} catch (std::runtime_error &err) {
		ERROR("Reading config file " << opts.configFilePath <<"\n" << err.what());
	}
}



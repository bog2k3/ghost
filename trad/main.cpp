/*
 * main.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: bog
 */


#include "daemon.h"
#include "../common/log.h"
#include "../common/dir.h"

#include <iostream>
#include <string>

bool parseCmdLine(int argc, char* argv[],
		std::string &out_sport,
		std::string &out_configPath,
		std::string &out_listePath,
		std::string &out_cachePath,
		std::vector<std::string> &out_nume) {
	bool sportParamExist = false;
	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i], "--sport")) {
			if (i == argc-1) {
				ERROR("Așteptam un argument dupa '--sport' !!!");
				return false;
			}
			out_sport = argv[++i];
			sportParamExist = true;
		}
		else if (!strcmp(argv[i], "--config")) {
			if (i == argc-1) {
				ERROR("Așteptam un argument după '--config' !!!");
				return false;
			}
			out_configPath = argv[++i];
		}
		else if (!strcmp(argv[i], "--lpath")) {
			if (i == argc-1) {
				ERROR("Așteptam un argument după '--lpath' !!!");
				return false;
			}
			out_listePath = argv[++i];
		}
		else if (!strcmp(argv[i], "--cpath")) {
			if (i == argc-1) {
				ERROR("Așteptam un argument după '--cpath' !!!");
				return false;
			}
			out_cachePath = argv[++i];
		}
		else {
			out_nume.push_back(argv[i]);
		}
	}
	if (!sportParamExist)
		ERROR("Lipsește parametrul '--sport' !!!");
	return sportParamExist;
}

int main(int argc, char* argv[]) {
	std::string sport;
	std::string configPath;
	std::string listePath = ".";
	std::string cachePath = ".";
	std::vector<std::string> nume;
	// TODO inlocuit cu aia standard din cmdLine.h
	if (!parseCmdLine(argc, argv, sport, configPath, listePath, cachePath, nume) || configPath.empty()) {
		ERROR("Sintaxa liniei de comanda este proasta!\n" <<
				"Exemplu bun: \n" <<
				"trad --config ~/.ghost.config --sport fotbal [--lpath path] [--cpath path] \"Echipa unu\" \"echipa doi\" ...\n");
		return -1;
	}

	if (!pathExists(cachePath+"/.trad"))
		mkDirRecursive(cachePath+"/.trad");
#ifndef DEBUG
	std::ofstream logFile(cachePath + "/.trad/trad.log", std::ios::app);
	logger::setLogStream(&logFile);
#endif
	std::ofstream errFile(cachePath + "/.trad/trad_err.log", std::ios::app);
	logger::setAdditionalErrStream(&errFile);

	Daemon daemon(configPath, listePath, cachePath);

	auto traduse = daemon.match(nume, sport);
	for (unsigned i=0; i<traduse.size(); i++)
		std::cout << traduse[i] << "\n";

	return 0;
}


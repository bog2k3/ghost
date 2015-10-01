/*
 * cmdLine.h
 *
 *  Created on: Oct 1, 2015
 *      Author: bog
 */

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <string>

struct cmdLineOptions {
	std::string tableName;
	std::string configFilePath;
};

void parseCommandLine(cmdLineOptions &opts, int argc, char* argv[]);

#endif /* CMDLINE_H_ */

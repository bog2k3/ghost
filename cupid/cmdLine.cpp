/*
 * cmdLine.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: bog
 */

#include "cmdLine.h"
#include "../cpplib/log.h"
#include <cstring>


#define EXPECT_ARG_AFTER(X, argc, i) if (argc == i+1) { \
	ERROR("asteptam un argument dupa " << X); \
	exit(-1); \
}

void parseCommandLine(cmdLineOptions &opts, int argc, char* argv[]) {
	for (int i=0; i<argc; i++) {
		if (!strcmp(argv[i], "--table")) {
			EXPECT_ARG_AFTER("--table", argc, i);
			opts.tableName = argv[++i];
			i++;
		}
		if (!strcmp(argv[i], "--config")) {
			EXPECT_ARG_AFTER("--config", argc, i);
			opts.configFilePath = argv[++i];
			i++;
		}
	}
}

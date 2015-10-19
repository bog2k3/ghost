/*
 * cmdLine.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: bog
 */

#include "cmdLine.h"
#include "log.h"
#include <cstring>


bool parseCommandLine(int argc, char* argv[], std::map<std::string, std::string> &opts, std::vector<std::string> const& requiredOpts) {
	for (int i=0; i<argc; i++) {
		if (strstr(argv[i], "--") == argv[i]) {
			if (argc == i+1) {
				ERROR("asteptam un argument dupa " << argv[i]);
				return false;
			}
			opts[argv[i]+2] = argv[++i];
			i++;
		}
	}
	for (auto o : requiredOpts) {
		if (opts.find(o) == opts.end()) {
			ERROR("Lipseste argumentul --" << o);
			return false;
		}
	}
	return true;
}

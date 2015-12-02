/*
 * cmdLine.h
 *
 *  Created on: Oct 1, 2015
 *      Author: bog
 */

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <string>
#include <map>
#include <vector>

bool parseCommandLine(int argc, char* argv[], std::map<std::string, std::string> &opts, std::vector<std::string> const& requiredOpts);

#endif /* CMDLINE_H_ */

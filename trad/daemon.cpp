/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "daemon.h"
#include <fstream>

static constexpr auto DATAPATH = "~/.maimutza";
static constexpr auto DBPATH = DATAPATH + "/cache/";

Daemon::Daemon(std::string const& listePath)
	: listaPath(listePath)
	, ngramGenerator(3, false)
	, dbReader()
{
	refreshCache();
}

std::vector<std::string> Daemon::match(std::vector<std::string> const& nume) {

}

void Daemon::refreshCache() {
	std::ifstream db(DBPATH);
	if (!db.is_open())
}

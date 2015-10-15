/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "daemon.h"
#include "../cpplib/dir.h"
#include <fstream>

static const std::string DATAPATH = "~/.maimutza/";
static const std::string CACHEPATH = DATAPATH + "cache/";

Daemon::Daemon(std::string const& listePath)
	: listaPath_(listePath)
	, ngramGenerator_(3, false)
{
	refreshCache();
	loadCache();
}

std::vector<std::string> Daemon::match(std::vector<std::string> const& nume, std::string const& sport) {

}

void Daemon::reloadCacheFile(std::string const& path) {
	std::string sportName = stripExt(getFileName(path));
	sportData_[sportName].dbReader = std::make_shared<simstring::reader>();
	sportData_[sportName].dbReader->open(path);

	// TODO copiat lista in directorul de cache si incarcat alternateMap din ea
}

void Daemon::loadCache() {
	std::vector<std::string> cacheFiles = getFiles(CACHEPATH);
	for (auto f : cacheFiles) {
		reloadCacheFile(f);
	}
}

void Daemon::updateCacheFile(std::string const& listPath, std::string const& cachePath) {
	simstring::writer_base<std::string> dbw(ngramGenerator_, cachePath);

	dbw.insert(L"Concordia Chiajna");

	dbw.close();

	reloadCacheFile(cachePath);
}

void Daemon::refreshCache() {
	std::vector<std::string> listFiles = getFiles(listaPath_);
	for (auto f : listFiles) {
		if (isDir(f))
			continue;
		// check if list cache exists and is up-to-date with the file:
		std::string cachePath = CACHEPATH + getFileName(f);
		bool cacheNeedsUpdate = false;
		if (!pathExists(cachePath))
			cacheNeedsUpdate = true;
		else {
			// check timestamps
			unsigned long tsf = getFileTimestamp(f);
			unsigned long tsc = getFileTimestamp(cachePath);
			if (tsc < tsf)
				cacheNeedsUpdate = true;
		}
		if (cacheNeedsUpdate)
			updateCacheFile(f, cachePath);
	}
}

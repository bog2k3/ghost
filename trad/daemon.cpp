/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "daemon.h"
#include "../cpplib/dir.h"
#include "../cpplib/log.h"
#include "../cpplib/split.h"
#include "../cpplib/assert.h"
#include <fstream>

//#define ENABLE_COSINE_MATCHING

static const std::string DATAPATH = "~/.maimutza/";
static const std::string CACHEPATH = DATAPATH + "cache/";

Daemon::Daemon(std::string const& listePath)
	: pathListe_(listePath)
	, ngramGenerator_(3, false)
{
	refreshCache();
	loadCache();
}

std::vector<std::string> Daemon::match(std::vector<std::string> const& nume, std::string const& sport) {
	std::vector<std::string> ret;

	for (int i=0; i<nume.size(); i++) {
		// Retrieve similar strings into a string vector.
		std::vector<std::string> xstrs;
		float thresh = 0.5f;
		// try overlap first:
		do {
			xstrs.clear();
			sportData_[sport].dbReader->retrieve(nume[i], simstring::overlap, thresh, std::back_inserter(xstrs));
			if (xstrs.size() > 1)
				thresh += 0.03f;
		} while (xstrs.size() > 1);
#ifdef ENABLE_COSINE_MATCHING
		if (!xstrs.size()) {
			// try cosine dist:
			thresh = 0.5f;
			do {
				xstrs.clear();
				sportData_[sport].dbReader->retrieve(nume[i], simstring::cosine, thresh, std::back_inserter(xstrs));
				if (xstrs.size() > 1)
					thresh += 0.03f;
			} while (xstrs.size() > 1);
		}
#endif
		if (xstrs.size())
			ret.push_back(sportData_[sport].alternateNameMap[xstrs[0]]);
		else
			ret.push_back("NECUNOSCUT");
	}
	assert(ret.size() == nume.size());
	return ret;
}

void Daemon::reloadCacheFile(std::string const& path) {
	std::string sportName = stripExt(getFileName(path));
	sportData_[sportName].dbReader = std::make_shared<simstring::reader>();
	sportData_[sportName].dbReader->open(path);
	if (sportData_[sportName].dbReader->fail()) {
		ERROR("Nu s-a putut incarca cache-ul \"" << sportData_[sportName].dbReader->error() << "\" !!!");
	}

	sportData_[sportName].alternateNameMap.clear();
	std::ifstream flist(stripExt(path));
	if (!flist.is_open()) {
		ERROR("Nu am putut deschide fisierul lista \"" << stripExt(path) << "\"!!!");
		return;
	}
	// updatam mapa de nume alternative:
	std::string line;
	while (std::getline(flist, line)) {
		std::vector<std::string> nume = strSplit(line, ';');
		for (int i=0; i<nume.size(); i++) {
			if (nume[i].empty())
				continue;
			sportData_[sportName].alternateNameMap[nume[i]] = nume[0];
		}
	}
}

void Daemon::loadCache() {
	std::vector<std::string> cacheFiles = getFiles(CACHEPATH);
	for (auto f : cacheFiles) {
		reloadCacheFile(f);
	}
}

void Daemon::updateCacheFile(std::string const& listPath, std::string const& cachePath) {
	if (pathExists(cachePath))
		if (!deleteFile(cachePath)) {
			ERROR("Nu s-a putut sterge cache-ul vechi!!! \"" << cachePath << "\""
					<< "\nCache-ul nu va fi updatat !!!");
			return;
		}
	simstring::writer_base<std::string> dbw(ngramGenerator_, cachePath);

	std::ifstream flist(listPath);
	if (!flist.is_open()) {
		ERROR("Nu am putut deschide fisierul lista \"" << listPath << "\"!!!");
		dbw.close();
		return;
	}
	std::string line;
	while (std::getline(flist, line)) {
		std::vector<std::string> nume = strSplit(line, ';');
		for (int i=0; i<nume.size(); i++)
			dbw.insert(nume[i]);
	}
	dbw.close();

	// copiem lista:
	if (!copyFile(listPath, stripExt(cachePath))) {
		ERROR("Nu s-a putut copia lista in directorul de cache !!!");
	}
	// reincarcam cache-ul respectiv:
	reloadCacheFile(cachePath);
}

void Daemon::refreshCache() {
	std::vector<std::string> listFiles = getFiles(pathListe_);
	for (auto f : listFiles) {
		if (isDir(f))
			continue;
		// check if list cache exists and is up-to-date with the file:
		std::string cachePath = CACHEPATH + stripExt(getFileName(f)) + ".db";
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

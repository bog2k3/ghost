/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "daemon.h"
#include "../common/dir.h"
#include "../common/log.h"
#include "../common/wstrManip.h"
#include "../common/assert.h"
#include <fstream>
#include <algorithm>
#include <locale>

//#define ENABLE_COSINE_MATCHING

std::string getDataPath() {
	return std::string(getenv("HOME")) + "/.maimutza/";
}
std::string getCachePath() {
	return getDataPath() + "cache/";
}

Daemon::Daemon(std::string const& listePath)
	: pathListe_(listePath)
	, ngramGenerator_(2, false)	// 2 sau 3, vedem cu care e mai bine
{
	refreshCache();
	loadCache();
}

std::vector<std::wstring> Daemon::match(std::vector<std::wstring> const& nume, std::string const& sport) {
	std::vector<std::wstring> ret;

	if (sportData_[sport].dbReader == nullptr) {
		ERROR("Nu exista lista pentru " << sport);
		ret.assign(nume.size(), L"NECUNOSCUT");
		return ret;
	}

	for (unsigned i=0; i<nume.size(); i++) {
		std::wstring crt = nume[i];
		wToLower(crt);
		bool u19 = false, u21 = false;
		size_t u19pos = 0, u21pos = 0;
		if ((u19pos = crt.find(L"u19")) != crt.npos) {
			u19 = true;
			crt = crt.substr(0, u19pos) + crt.substr(u19pos+3);
		}
		if ((u19pos = crt.find(L"u 19")) != crt.npos) {
			u19 = true;
			crt = crt.substr(0, u19pos) + crt.substr(u19pos+4);
		}
		if ((u21pos = crt.find(L"u21")) != crt.npos) {
			u21 = true;
			crt = crt.substr(0, u21pos) + crt.substr(u21pos+3);
		}
		if ((u21pos = crt.find(L"u 21")) != crt.npos) {
			u21 = true;
			crt = crt.substr(0, u21pos) + crt.substr(u21pos+4);
		}
		// Retrieve similar strings into a string vector.
		std::vector<std::wstring> xstrs;
		float thresh = 0.5f;
		// try overlap first:
		do {
			xstrs.clear();
			sportData_[sport].dbReader->retrieve(crt, simstring::overlap, thresh, std::back_inserter(xstrs));
			if (xstrs.size() > 1)
				thresh += 0.03f;
		} while (xstrs.size() > 1 && thresh < 0.9f);
#ifdef ENABLE_COSINE_MATCHING
		if (!xstrs.size()) {
			// try cosine dist:
			thresh = 0.5f;
			do {
				xstrs.clear();
				sportData_[sport].dbReader->retrieve(crt, simstring::cosine, thresh, std::back_inserter(xstrs));
				if (xstrs.size() > 1)
					thresh += 0.03f;
			} while (xstrs.size() > 1);
		}
#endif
		if (xstrs.size()) {
			std::wstring numeStd = sportData_[sport].alternateNameMap[xstrs[0]];
			if (u19)
				numeStd += L" U19";
			if (u21)
				numeStd += L" U21";
			ret.push_back(numeStd);
		} else
			ret.push_back(L"NECUNOSCUT");
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
	std::wifstream flist(stripExt(path));
	if (!flist.is_open()) {
		ERROR("Nu am putut deschide fisierul lista \"" << stripExt(path) << "\"!!!");
		return;
	}
	std::locale loc("en_US.UTF8");
	flist.imbue(loc);
	// updatam mapa de nume alternative:
	std::wstring line;
	while (std::getline(flist, line)) {
		std::vector<std::wstring> nume = wstrSplit(line, L';');
		for (int i=0; i<nume.size(); i++) {
			if (nume[i].empty())
				continue;
			sportData_[sportName].alternateNameMap[nume[i]] = nume[0];
		}
	}
}

void Daemon::loadCache() {
	std::vector<std::string> cacheFiles = getFiles(getCachePath());
	for (auto f : cacheFiles) {
		if (getFileExt(f) == ".db")
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
	simstring::writer_base<std::wstring> dbw(ngramGenerator_, cachePath);

	std::wifstream flist(listPath);
	if (!flist.is_open()) {
		ERROR("Nu am putut deschide fisierul lista \"" << listPath << "\"!!!");
		dbw.close();
		return;
	}
	std::locale loc("en_US.UTF8");
	flist.imbue(loc);
	std::wstring line;
	while (std::getline(flist, line)) {
		std::vector<std::wstring> nume = wstrSplit(line, L';');
		for (unsigned i=0; i<nume.size(); i++)
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
	// verificam daca exista directorul de cache:
	if (!pathExists(getDataPath()))
		if (!mkDir(getDataPath()))
			return;
	if (!pathExists(getCachePath()))
		if (!mkDir(getCachePath()))
			return;
	std::vector<std::string> listFiles = getFiles(pathListe_);
	for (auto f : listFiles) {
		if (isDir(f))
			continue;
		// check if list cache exists and is up-to-date with the file:
		std::string cachePath = getCachePath() + stripExt(getFileName(f)) + ".db";
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

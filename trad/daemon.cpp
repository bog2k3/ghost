/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "daemon.h"
#include "../common/sanitize.h"
#include "../common/strManip.h"
#include "../common/dir.h"
#include "../common/log.h"
#include "../common/assert.h"
#include "../common/configFile.h"
#include "../common/eMailer.h"

#include <fstream>
#include <algorithm>

//#define ENABLE_COSINE_MATCHING

struct {
	const char* muser = "mail-user";
	const char* mpassw = "mail-passw";
	const char* mserver = "mail-server";
	const char* maddr = "mail-addr";
	const char* mfrom = "mail-from";
	const char* mdest = "mail-dest";
	const char* msign = "mail-signature";
} Cfg;

Daemon::Daemon(std::string const& configPath, std::string const& listePath, std::string const& dataPath)
	: pathListe_(listePath)
	, dataPath_(dataPath + "/.trad")
	, ngramGenerator_(2, false)	// 2 sau 3, vedem cu care e mai bine
{
	refreshCache();
	loadCache();

	std::map<std::string, std::string> cfgOpt;
	if (!parseConfigFile(configPath, cfgOpt, {Cfg.muser, Cfg.mpassw, Cfg.mserver, Cfg.maddr, Cfg.mdest})) {
		ERROR("Trad Daemon nu poate trimite email!!!");
		return;
	}
	pEmailer = new EMailer(cfgOpt[Cfg.mserver], cfgOpt[Cfg.muser], cfgOpt[Cfg.mpassw], cfgOpt[Cfg.maddr], cfgOpt[Cfg.mfrom], cfgOpt[Cfg.msign]);
	emailDest = strSplit(cfgOpt[Cfg.mdest], ',');
}

std::string Daemon::getCachePath() {
	return dataPath_ + "/cache/";
}

void Daemon::reportFailed(std::vector<std::string> const& failed) {
	std::string failedFilePath = dataPath_ + "/failed";
	std::string failedTSPath = dataPath_ + "/failed.last";

	if (!pathExists(failedTSPath)) {
		if (!touchFile(failedTSPath))
			ERROR("Nu am putut crea/updata [touch] fisierul " << failedTSPath);
	}

	std::ofstream ffile(failedFilePath, std::ios::app);
	if (!ffile.is_open()) {
		ERROR("Nu am putut deschide [append] fisierul cu echipe failate: " << failedFilePath);
		return;
	}
	for (auto s : failed)
		ffile << s << "\n";
	ffile.close();

	if (getFileTimestamp(failedTSPath) + 3600*24 <= getFileTimestamp(failedFilePath)) {
		if (!pEmailer) {
			ERROR("Email nu e configurat, nu pot sa trimit lista cu failate!!!");
			return;
		}
		// au trecut cel putin 24h de la ultima notificare pe mail
		std::ifstream fFailed(failedFilePath);
		if (!fFailed.is_open()) {
			ERROR("Nu am putut deschide [read] fisierul cu echipe failate: " << failedFilePath);
			return;
		}

		if (!touchFile(failedTSPath)) {
			ERROR("Nu am putut updata [touch] fisierul " << failedTSPath);
		}
		std::stringstream ss;
		ss << "Cuvinte cu diacritice necunoscute:\r\n";
		std::string line;
		while (std::getline(fFailed, line)) {
			ss << line << "\r\n";
		}
		fFailed.close();

		pEmailer->send(emailDest, "traduceri FAILATE", ss.str());
		deleteFile(failedFilePath);
	}
}

std::vector<std::string> Daemon::match(std::vector<std::string> const& numeIn, std::string const& sport) {
	std::vector<std::string> ret;

	if (sportData_[sport].dbReader == nullptr) {
		ERROR("Nu exista lista pentru " << sport);
		ret.assign(numeIn.size(), "NECUNOSCUT");
		return ret;
	}

	auto nume = numeIn;
	std::vector<sanitizeResult> sanRes = sanitize(nume);
	std::vector<std::string> failed;

	for (unsigned i=0; i<nume.size(); i++) {
		std::string crt = nume[i];
		if (sanRes[i].failedDiacritics) {
			failed.push_back(nume[i]);
			ret.push_back("NECUNOSCUT");
			continue;
		}

		// Retrieve similar strings into a string vector.
		std::vector<std::string> xstrs;
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
			std::string numeStd = sportData_[sport].alternateNameMap[xstrs[0]];
			if (sanRes[i].age_u19)
				numeStd += " U19";
			if (sanRes[i].age_u21)
				numeStd += " U21";
			ret.push_back(numeStd);
		} else
			ret.push_back("NECUNOSCUT");
	}
	assert(ret.size() == nume.size());

	// punem tot ce e in failed intr-un fisier si o data pe zi trimit mail cu el - trebuie adaugate diacritice
	reportFailed(failed);

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
	simstring::writer_base<std::string> dbw(ngramGenerator_);
	if (!dbw.open(cachePath)) {
		ERROR("Nu s-a putut accesa cache-ul la :" << cachePath);
		return;
	}

	std::ifstream flist(listPath);
	if (!flist.is_open()) {
		ERROR("Nu am putut deschide fisierul lista \"" << listPath << "\"!!!");
		dbw.close();
		return;
	}
	std::string line;
	while (std::getline(flist, line)) {
		std::vector<std::string> nume = strSplit(line, ';');
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
	if (!pathExists(getCachePath()))
		if (!mkDirRecursive(getCachePath())) {
			ERROR("Nu s-a putut crea directorul de cache: " << getCachePath());
			return;
		}
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

/*
 * daemon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#ifndef DAEMON_CPP_
#define DAEMON_CPP_

#include <simstring/simstring.h>
#include <string>
#include <vector>
#include <memory>

class Daemon {
public:
	// initializeaza demonul.
	// listePath - path-ul catre directorul cu liste de echipe/jucatori
	// se citesc toate fisierele din director, indiferent de nume
	Daemon(std::string const& listePath);

	// traduce o lista de nume.
	// Cauta in listele cu echipe una care sa fie egala sau aproximativ egala;
	// Daca nu se gaseste nici una, intoarce "NECUNOSCUT" pe pozitia respectiva
	std::vector<std::wstring> match(std::vector<std::wstring> const& nume, std::string const& sport);

	// verifica daca s-au updatat listele cu echipe si updateaza cache-ul
	void refreshCache();

private:
	std::string pathListe_;
	simstring::ngram_generator ngramGenerator_;

	struct sportData {
		std::shared_ptr<simstring::reader> dbReader;
		std::map<std::wstring, std::wstring> alternateNameMap;
	};

	std::map<std::string, sportData> sportData_;	// maps sport names to db readers (which take a single sport-specific list file as input) & alternate name mappings

	void loadCache();
	void reloadCacheFile(std::string const& path);
	void updateCacheFile(std::string const& listPath, std::string const& cachePath);
};



#endif /* DAEMON_CPP_ */

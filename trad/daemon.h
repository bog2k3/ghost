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

class Daemon {
public:
	// initializeaza demonul.
	// listePath - path-ul catre directorul de liste cu echipe
	// directorul cu liste are forma urmatoare:
	// dir					[D]
	//  |
	//  |--sport1			[D]
	//  |    |--lista		[f]
	//  |    |--lista-u19	[f]
	//  |    |--lista-u21	[f]
	//  |--sport2			[D]
	//  .......................
	Daemon(std::string const& listePath);

	// traduce o lista de nume.
	// Cauta in listele cu echipe una care sa fie egala sau aproximativ egala;
	// Daca nu se gaseste nici una, intoarce "NECUNOSCUT" pe pozitia respectiva
	std::vector<std::string> match(std::vector<std::string> const& nume, std::string sport);

	// verifica daca s-au updatat listele cu echipe si updateaza cache-ul
	void refreshCache();

private:
	std::string listaPath;
	simstring::ngram_generator ngramGenerator;
	simstring::reader dbReader;
};



#endif /* DAEMON_CPP_ */

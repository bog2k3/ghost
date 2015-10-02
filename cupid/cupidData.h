/*
 * cupidData.h
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#ifndef CUPIDDATA_H_
#define CUPIDDATA_H_

#include <vector>
#include <map>
#include <cstdint>

class iChelner;

struct cupidGameData {
	std::string internalGameId;	// identificator unic intern (in DB) al unui joc,
							// determinat de o combinatie de 3 parametrii:
					 		// numele echipei A, numele echipei B, timpul de joc
	std::string gameId;		// id-ul jocului de pe site (ca sa stim ce-i zicem site-ului cand dam comanda)
	double cota1 = 0;		// cota pentru A
	double cota1x = 0;		// cota pentru Ax
	double cota2 = 0;		// cota pentru B
	double cota2x = 0;		// cota pentru Bx
	double cota12 = 0;		// cota pt A sau B
	double cotax = 0;		// cota pt egal
	std::string siteId = 0;	// id-ul site-ului -> cautam metoda de aplicare intr-o mapa

	cupidGameData() = default;
};

struct cupidSiteData {
	iChelner* chelner;	// chelnerul care primeste comenzi pentru site-ul asta
	// daca mai tre sa tinem si alte informatii despre site, bagam aici
	// ...
};

struct cupidPair
{
	unsigned index1;	// pereche de indexi in vectorul de jocuri pentru o meciuire.
	unsigned index2;
	double ratio;		// raportul in care trebuie pariat (ratio = suma1 / suma2) pentru profit maxim
};

extern std::vector<cupidGameData> gameData; // vectorul de jocuri - vine din baza de date
extern std::vector<cupidPair> pairs; // vectorul de meciuiri intre jocuri pe care punem comenzi
extern std::map<std::string, cupidSiteData> mapSites; // mapa de id-uri de site -> date despre site

#endif /* CUPIDDATA_H_ */

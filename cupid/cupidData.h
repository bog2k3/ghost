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

enum class pariu {
	P1,
	P1X,
	PX,
	PX2,
	P2,
	P12,
};

struct cupidGameData {
	std::string internalGameId = "";// identificator unic intern (in DB) al unui joc,
							// determinat de o combinatie de 3 parametrii:
					 		// numele echipei A, numele echipei B, timpul de joc
	std::string gameId = "";// id-ul jocului de pe site (ca sa stim ce-i zicem site-ului cand dam comanda)
	std::map<pariu, double> cote;	// cotele
	std::string siteId = "";// id-ul site-ului -> cautam metoda de aplicare intr-o mapa

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
	pariu p1;
	unsigned index2;
	pariu p2;
	double ratio;		// raportul in care trebuie pariat (ratio = suma1 / suma2) pentru profit maxim
};

extern std::vector<cupidGameData> gameData; // vectorul de jocuri - vine din baza de date
extern std::vector<cupidPair> pairs; // vectorul de meciuiri intre jocuri pe care punem comenzi
extern std::map<std::string, cupidSiteData> mapSites; // mapa de id-uri de site -> date despre site

#endif /* CUPIDDATA_H_ */

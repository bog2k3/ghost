/*
 * main.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: bog
 */


#include "../cpplib/log.h"
#include <iostream>
#include <locale>
#include <string>
#include <simstring/simstring.h>

void retrieve(
    simstring::reader& dbr,
    const std::wstring& query,
    int measure,
    double threshold
    )
{
    // Retrieve similar strings into a string vector.
    std::vector<std::wstring> xstrs;
    dbr.retrieve(query, measure, threshold, std::back_inserter(xstrs));

    // Output the retrieved strings separated by ", ".
    for (int i = 0;i < (int)xstrs.size();++i) {
        std::wcout << xstrs[i] << L"\n";
    }
    std::wcout << std::endl;
}

bool parseCmdLine(int argc, char* argv[], std::string &out_sport, std::vector<std::string> &out_nume) {
	bool sportParamExist = false;
	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i], "--sport")) {
			if (i == argc-1) {
				ERROR("Așteptam un argument dupa '--sport' !!!");
				return false;
			}
			out_sport = argv[++i];
			sportParamExist = true;
			i++;
		}
		else
			out_nume.push_back(argv[i]);
	}
	if (!sportParamExist)
		ERROR("Lipsește parametrul '--sport' !!!");
	return sportParamExist;
}

int main(int argc, char* argv[]) {
	std::locale::global(std::locale(""));
	std::cout.imbue(std::locale(""));

	std::string sport;
	std::vector<std::string> nume;
	if (!parseCmdLine(argc, argv, sport, nume)) {
		ERROR("Sintaxa liniei de comanda este proasta!\n" <<
				"Exemplu bun: \n" <<
				"trad --sport fotbal \"Echipa unu\" \"echipa doi\" ...\n");
		return -1;
	}

	if (!loadListe())

	// Open a SimString database for writing (with std::wstring).
	simstring::ngram_generator gen(3, false);
	simstring::writer_base<std::wstring> dbw(gen, "sample_unicode.db");
	dbw.insert(L"Concordia Chiajna");
	dbw.insert(L"concordia chiajna");
	dbw.insert(L"fcm braila");
	dbw.insert(L"fc municipal buzau");
	dbw.insert(L"buzau");

	dbw.insert(L"Steaua Resita");
	dbw.insert(L"Steaua Moscova");
	dbw.insert(L"Dinamo Bucuresti");
	dbw.insert(L"Rapid Bucuresti");
	dbw.insert(L"Sportul Studentesc Bucuresti");
	dbw.close();

	// Open the database for reading.
	simstring::reader dbr;
	dbr.open("sample_unicode.db");

	// Output similar strings from Unicode queries.
	retrieve(dbr, L"fcm buzau", simstring::overlap, 0.5);

	return 0;
}


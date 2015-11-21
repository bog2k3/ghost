/*
 * DummySQLSock.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#include "DummySQLSock.h"
#include "../common/strManip.h"
#include "../common/log.h"
#include <fstream>

DummySQLSock::DummySQLSock(const char* filePath) {
	LOGPREFIX("DummySQLSock");
	std::ifstream f(filePath);
	if (!f.is_open()) {
		ERROR("nu s-a putut deschide fisierul baza de date \"" << filePath << "\"");
		return;
	}
	std::string cnames;
	std::getline(f, cnames);
	auto labels = strSplit(cnames,';');
	nColoane = labels.size();
	for (int i=0; i<nColoane; i++) {
		indexColoane[i] = labels[i];
		coloane[labels[i]];
	}
	std::string l;
	while (std::getline(f, l)) {
		if (l.empty())
			continue;
		auto valori = strSplit(l, ';');
		insert(valori);
	}
}

DummySQLSock::~DummySQLSock() {
	// TODO Auto-generated destructor stub
}

void DummySQLSock::insert(std::vector<std::string> const& val) {
	for (int i=0; i<nColoane; i++)
		coloane[indexColoane[i]].push_back(i < val.size() ? val[i] : "");
}

bool DummySQLSock::connect(std::string const& URI, std::string const& user, std::string const& passw) {
	return true;
}

bool DummySQLSock::setDB(std::string const& databaseName) {
	return true;
}

std::unique_ptr<sql::ResultSet> DummySQLSock::doQuery(std::string const& query) {
	// here goes dragons - parse SQL syntax :-)
}

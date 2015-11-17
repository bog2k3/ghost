/*
 * listFile.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: bog
 */

#include "listFile.h"
#include "../common/strManip.h"
#include "../common/log.h"
#include <algorithm>
#include <fstream>

void listFile::addNewAlias(std::string key, std::string alias) {
	auto line = names2Lines.find(key);
	auto betterKey = [](std::string const& left, std::string const& right) -> bool {
		unsigned nwl = getNumberOfWords(left),
			nwr = getNumberOfWords(right);
		return nwl > nwr || (nwl == nwr && left.length() > right.length());
	};
	if (line == names2Lines.end()) {
		// inregistrare noua
		if (betterKey(alias, key)) {
			// exchange key and alias
			std::string aux = key;
			key = alias;
			alias = aux;
		}
		lines.push_back({key, alias});
		names2Lines[key] = names2Lines[alias] = lines.size() - 1;
	} else {
		auto &crtLine = lines[line->second];
		crtLine.push_back(alias);
		std::sort(crtLine.begin(), crtLine.end(), betterKey);
	}
}

listFile loadListFile(std::string const& filename) {
	std::ifstream f(filename);
	listFile lf;
	if (!f.is_open()) {
		ERROR("Nu s-a putut deschide (mode:R) fisierul \"" << filename << "\"");
		return lf;
	}
	lf.io_result = listFile::IO_OK;
	std::string line;
	while (std::getline(f, line)) {
		lf.lines.push_back(strSplit(line,';'));
		for (auto tk : lf.lines.back())
			lf.names2Lines[tk] = lf.lines.size() - 1;
	}
	return lf;
}

listFile::IORESULT saveListFile(std::string const& filename, listFile& list) {
	std::ofstream f(filename);
	if (!f.is_open()) {
		ERROR("Nu s-a putut deschide (mode:W) fisierul \"" << filename << "\"");
		return listFile::IO_FAIL;
	}
	for (auto l : list.lines) {
		f << l[0];
		for (unsigned i=1; i<l.size(); i++)
			f << ';' << l[i];
		f << '\n';
	}
	return listFile::IO_OK;
}

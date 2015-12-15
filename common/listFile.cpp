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
	auto betterKey = [](std::string const& left, std::string const& right) -> bool {
		unsigned nwl = getNumberOfWords(left),
			nwr = getNumberOfWords(right);
		return nwl > nwr || (nwl == nwr && left.length() > right.length());
	};
	if (betterKey(alias, key)) {
		std::string aux = key;
		key = alias;
		alias = aux;
	}
	auto line = names2Lines.find(key);
	if (line == names2Lines.end()) {
		// inregistrare noua
		if (betterKey(alias, key)) {
			// exchange key and alias
			std::string aux = key;
			key = alias;
			alias = aux;
		}
		if (key!=alias)
			lines.push_back({key, alias});
		else
			lines.push_back({key});
		names2Lines[key].insert(lines.size() - 1);
		names2Lines[alias].insert(lines.size() - 1);
	} else {
		auto &crtLine = lines[*line->second.begin()];
		// check if not present already, then add:
		if (std::find(crtLine.begin(), crtLine.end(), alias) == crtLine.end()) {
			crtLine.push_back(alias);
			std::sort(crtLine.begin(), crtLine.end(), betterKey);
		}
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
			lf.names2Lines[tk].insert(lf.lines.size() - 1);
	}
	return lf;
}

static void mergeLines(listFile &list, std::vector<std::set<int>> const& mergeSets) {
	for (auto s : mergeSets) {
		int keyLine = -1;
		for (auto l : s) {
			if (list.lines[l].empty())
				continue;
			if (keyLine == -1) {
				keyLine = l;
				continue;
			}
			for (auto w : list.lines[l])
				list.addNewAlias(list.lines[keyLine][0], w);

			list.lines[l].clear();
		}
	}
}

static void trimEmptyLines(listFile &list) {
	for (uint i=0; i<list.lines.size(); i++) {
		if (list.lines[i].empty()) {
			list.lines.erase(list.lines.begin()+(i--));
		}
	}
}

listFile::IORESULT saveListFile(std::string const& filename, listFile& list) {
	// prima data compactam lista - daca exista duplicate, le merge-uim intr-o singura linie
	std::vector<std::set<int>> mergeSets;
	for (auto &rec : list.names2Lines) {
		if (rec.second.size() > 1)
			mergeSets.push_back(rec.second);
	}
	mergeLines(list, mergeSets);
	trimEmptyLines(list);

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

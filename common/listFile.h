/*
 * listFile.h
 *
 *  Created on: Nov 17, 2015
 *      Author: bog
 */

#ifndef LISTFILE_H_
#define LISTFILE_H_

#include <string>
#include <vector>
#include <map>
#include <set>

struct listFile {
	enum IORESULT {
		IO_FAIL,
		IO_OK
	} io_result = IO_FAIL;

	std::map<std::string, std::set<int>> names2Lines;	// maps each variation of a name to its lines in the list file
	std::vector<std::vector<std::string>> lines;	// each line is a vector<string> with alternate names

	// key is the new/existing key, alias the new alis.
	// if the key doesn't already exist, a new record is created
	void addNewAlias(std::string key, std::string alias);
};

listFile loadListFile(std::string const& filename);
listFile::IORESULT saveListFile(std::string const& filename, listFile& list);

#endif /* LISTFILE_H_ */

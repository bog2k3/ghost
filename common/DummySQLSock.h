/*
 * DummySQLSock.h
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#ifndef DUMMYSQLSOCK_H_
#define DUMMYSQLSOCK_H_

#include "ISQLSock.h"
#include <vector>
#include <map>
#include <string>

class DummySQLSock: public ISQLSock {
public:
	DummySQLSock(const char* filePath);
	virtual ~DummySQLSock() override;

	virtual bool connect(std::string const& URI, std::string const& user, std::string const& passw) override;
	virtual bool setDB(std::string const& databaseName) override;

	virtual std::unique_ptr<sql::ResultSet> doQuery(std::string const& query) override;

private:
	int nRecords = 0;
	int nColoane = 0;
	std::map<std::string, std::vector<std::string>> coloane;
	std::map<int, std::string> indexColoane;

	void insert(std::vector<std::string> const& val);
};

#endif /* DUMMYSQLSOCK_H_ */

/*
 * DummySQLSock.h
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#ifndef DUMMYSQLSOCK_H_
#define DUMMYSQLSOCK_H_

#include "ISQLSock.h"

class DummySQLSock: public ISQLSock {
public:
	DummySQLSock(const char* filePath);
	virtual ~DummySQLSock() override;

	virtual bool connect(std::string const& URI, std::string const& user, std::string const& passw) override;
	virtual bool setDB(std::string const& databaseName) override;

	virtual std::unique_ptr<sql::ResultSet> doQuery(std::string const& query) override;
};

#endif /* DUMMYSQLSOCK_H_ */

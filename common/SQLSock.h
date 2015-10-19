/*
 * SQLSock.h
 *
 *  Created on: Oct 2, 2015
 *      Author: bog
 */

#ifndef SQLSOCK_H_
#define SQLSOCK_H_

#include "ISQLSock.h"
#include <cppconn/driver.h>
#include <memory>

class SQLSock : public ISQLSock {
public:
	SQLSock();
	virtual ~SQLSock() override;

	bool connect(std::string const& URI, std::string const& user, std::string const& passw) override;
	bool setDB(std::string const& databaseName) override;

	std::unique_ptr<sql::ResultSet> doQuery(std::string const& query) override;

private:
	sql::Driver* mDriver = nullptr;
	sql::Connection* mConnection = nullptr;
};

#endif /* SQLSOCK_H_ */

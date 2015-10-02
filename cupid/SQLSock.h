/*
 * SQLSock.h
 *
 *  Created on: Oct 2, 2015
 *      Author: bog
 */

#ifndef SQLSOCK_H_
#define SQLSOCK_H_

#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <memory>

class SQLSock {
public:
	SQLSock();
	virtual ~SQLSock();

	bool connect(std::string const& URI, std::string const& user, std::string const& passw);
	bool setDB(std::string const& databaseName);

	std::unique_ptr<sql::ResultSet> doQuery(std::string const& query);

private:
	sql::Driver* mDriver = nullptr;
	sql::Connection* mConnection = nullptr;
};

#endif /* SQLSOCK_H_ */

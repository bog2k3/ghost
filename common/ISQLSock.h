/*
 * ISQLSock.h
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#ifndef ISQLSOCK_H_
#define ISQLSOCK_H_

#include <cppconn/resultset.h>
#include <memory>

class ISQLSock {
public:
	virtual ~ISQLSock() {}

	virtual bool connect(std::string const& URI, std::string const& user, std::string const& passw) = 0;
	virtual bool setDB(std::string const& databaseName) = 0;

	virtual std::unique_ptr<sql::ResultSet> doQuery(std::string const& query) = 0;
};

#endif /* ISQLSOCK_H_ */

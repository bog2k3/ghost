/*
 * SQLSock.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: bog
 */

#include "SQLSock.h"
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>

SQLSock::SQLSock() {
}

SQLSock::~SQLSock() {
	if (mConnection)
		delete mConnection;
}

bool SQLSock::connect(std::string const& URI, std::string const& user, std::string const& passw) {
	try {
		  /* Create a connection */
		  mDriver = get_driver_instance();
		  mConnection = mDriver->connect(URI, user, passw);

		  return true;
		} catch (sql::SQLException &e) {
			std::cerr << "# ERR: SQLException in " << __FILE__;
			std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
			std::cerr << "# ERR: " << e.what();
			std::cerr << " (MySQL error code: " << e.getErrorCode();
			std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;

			return false;
		}
}

bool SQLSock::setDB(std::string const& databaseName) {
	try {
		mConnection->setSchema(databaseName);

		return true;
	} catch (sql::SQLException &e) {
		std::cerr << "# ERR: SQLException in " << __FILE__;
		std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cerr << "# ERR: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return false;
	}
}

std::unique_ptr<sql::ResultSet> SQLSock::doQuery(std::string const& query) {
	try {
		sql::Statement *stmt;
		sql::ResultSet *res;
		stmt = mConnection->createStatement();
		res = stmt->executeQuery(query); // "select * from fotbal"
		delete stmt;
		return std::unique_ptr<sql::ResultSet>(res);
	} catch (sql::SQLException &e) {
		std::cerr << "# ERR: SQLException in " << __FILE__;
		std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cerr << "# ERR: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return nullptr;
	}
}

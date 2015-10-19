/*
 * DummySQLSock.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#include "DummySQLSock.h"

DummySQLSock::DummySQLSock(const char* filePath) {
	// TODO Auto-generated constructor stub

}

DummySQLSock::~DummySQLSock() {
	// TODO Auto-generated destructor stub
}

bool DummySQLSock::connect(std::string const& URI, std::string const& user, std::string const& passw) {

}

bool DummySQLSock::setDB(std::string const& databaseName) {

}

std::unique_ptr<sql::ResultSet> DummySQLSock::doQuery(std::string const& query) {

}

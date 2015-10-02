/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "SQLSock.h"
#include <stdio.h>
#include <string>

bool initializeazaDB(SQLSock &socket, std::string const& URI, std::string const& user, std::string const& passw, std::string const& dbName) {
	if (!socket.connect(URI, user, passw))
		return false;
	if (!socket.setDB(dbName))
		return false;

	return true;
}

void faQueryul(std::string tabel) {

}



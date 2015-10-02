/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "SQLSock.h"
#include "../cpplib/log.h"
#include <stdio.h>
#include <string>

bool initializeazaDB(SQLSock &socket, std::string const& URI, std::string const& user, std::string const& passw, std::string const& dbName) {
	if (!socket.connect(URI, user, passw))
		return false;
	if (!socket.setDB(dbName))
		return false;

	return true;
}

/*
--------------
describe fotbal
------------

+-----------+------------------+------+-----+-------------------+-----------------------------+
| Field     | Type             | Null | Key | Default           | Extra                       |
+-----------+------------------+------+-----+-------------------+-----------------------------+
| sesiune   | int(10) unsigned | NO   |     | NULL              |                             |
| echipa1   | varchar(32)      | YES  |     | NULL              |                             |
| echipa2   | varchar(32)      | YES  |     | NULL              |                             |
| cota_1    | float            | YES  |     | NULL              |                             |
| cota_2    | float            | YES  |     | NULL              |                             |
| cota_x    | float            | YES  |     | NULL              |                             |
| cota_1x   | float            | YES  |     | NULL              |                             |
| cota_2x   | float            | YES  |     | NULL              |                             |
| cota_12   | float            | YES  |     | NULL              |                             |
| site_id_1 | varchar(12)      | YES  |     | NULL              |                             |
| game_id_1 | varchar(12)      | YES  |     | NULL              |                             |
| timp      | timestamp        | NO   |     | CURRENT_TIMESTAMP | on update CURRENT_TIMESTAMP |
| hash_joc  | varchar(64)      | NO   | PRI |                   |                             |
+-----------+------------------+------+-----+-------------------+-----------------------------+
13 rows in set (0.01 sec)
--------------

 */

void faQueryul(SQLSock &socket, std::string tabel) {
	auto res = socket.doQuery("select * from " + tabel);

	int crtRecord = 0;
	while (res->next()) {
		LOG("record " << crtRecord++ << ": ");
		for (int i=1; i<=13; i++)
			LOGNP(res->getString(i) << ", ");
		LOGLN("");
	}
}



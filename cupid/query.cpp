/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "SQLSock.h"
#include "cupidData.h"
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
|  1 sesiune   | int(10) unsigned | NO   |     | NULL              |                             |
|  2 echipa1   | varchar(32)      | YES  |     | NULL              |                             |
|  3 echipa2   | varchar(32)      | YES  |     | NULL              |                             |
|  4 cota_1    | float            | YES  |     | NULL              |                             |
|  5 cota_2    | float            | YES  |     | NULL              |                             |
|  6 cota_x    | float            | YES  |     | NULL              |                             |
|  7 cota_1x   | float            | YES  |     | NULL              |                             |
|  8 cota_2x   | float            | YES  |     | NULL              |                             |
|  9 cota_12   | float            | YES  |     | NULL              |                             |
| 10 site_id_1 | varchar(12)      | YES  |     | NULL              |                             |
| 11 game_id_1 | varchar(12)      | YES  |     | NULL              |                             |
| 12 timp      | timestamp        | NO   |     | CURRENT_TIMESTAMP | on update CURRENT_TIMESTAMP |
| 13 hash_joc  | varchar(64)      | NO   | PRI |                   |                             |
+-----------+------------------+------+-----+-------------------+-----------------------------+
13 rows in set (0.01 sec)
--------------

 */

void faQueryul(SQLSock &socket, std::string tabel) {
	auto res = socket.doQuery("select * from " + tabel);

	while (res->next()) {
		cupidGameData data;
		data.internalGameId = res->getString(13);	// TODO - trebuie din coloana hash_joc_comun, nu hash_joc
		data.gameId = res->getString(11);
		data.cota1 = res->getDouble(4);
		data.cota1x = res->getDouble(7);
		data.cota2 = res->getDouble(5);
		data.cota2x = res->getDouble(8);
		data.cota12 = res->getDouble(9);
		data.cotax = res->getDouble(6);
		data.siteId = res->getString(10);

		gameData.push_back(data);
	}

	LOGLN("am tras " << gameData.size() << " inregistrari din baza de date.");
}



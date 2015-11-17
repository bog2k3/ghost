/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 *  data : 06-octombrie-2015 : Dan : adaugare sortare 
 */

#include "../common/SQLSock.h"
#include "../common/log.h"
#include "../common/dbLabels.h"
#include "cupidData.h"
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
--------------

+------------------+------------------+------+-----+-------------------+-----------------------------+
| Field            | Type             | Null | Key | Default           | Extra                       |
+------------------+------------------+------+-----+-------------------+-----------------------------+
| sesiune          | int(10) unsigned | NO   |     | NULL              |                             |
| echipa1          | varchar(32)      | YES  |     | NULL              |                             |
| echipa2          | varchar(32)      | YES  |     | NULL              |                             |
| cota_1           | float            | YES  |     | NULL              |                             |
| cota_2           | float            | YES  |     | NULL              |                             |
| cota_x           | float            | YES  |     | NULL              |                             |
| cota_1x          | float            | YES  |     | NULL              |                             |
| cota_2x          | float            | YES  |     | NULL              |                             |
| cota_12          | float            | YES  |     | NULL              |                             |
| site_id_1        | varchar(12)      | YES  |     | NULL              |                             |
| game_id_1        | varchar(12)      | YES  |     | NULL              |                             |
| timp             | timestamp        | NO   |     | CURRENT_TIMESTAMP | on update CURRENT_TIMESTAMP |
| hash_joc         | varchar(64)      | NO   | PRI |                   |                             |
| hash_joc_comun   | varchar(64)      | YES  |     | NULL              |                             |
| echipa1_internal | varchar(32)      | YES  |     | NULL              |                             |
| echipa2_internal | varchar(32)      | YES  |     | NULL              |                             |
| data_joc         | varchar(10)      | YES  |     | NULL              |                             |
| status_echipe    | int(1)           | YES  |     | NULL              |                             |
+------------------+------------------+------+-----+-------------------+-----------------------------+
18 rows in set (0.01 sec)

 */

void faQueryul(SQLSock &socket, std::string tabel) {

	// am adaugat ordonare, acum meciurile ce au acelasi hash comun vin unele dupa celelalte

	auto res = socket.doQuery("SELECT * FROM " + tabel + " WHERE " + dbLabels.statusTraduceri + "==0 ORDER BY "+dbLabels.hashComun);

	while (res->next()) {
		cupidGameData data;
		data.internalGameId = res->getString(dbLabels.hashComun);
		data.gameId = res->getString(dbLabels.gameId);
		data.cote[pariu::P1] = res->getDouble(dbLabels.cota1);
		data.cote[pariu::P1X] = res->getDouble(dbLabels.cota1x);
		data.cote[pariu::P2] = res->getDouble(dbLabels.cota2);
		data.cote[pariu::PX2] = res->getDouble(dbLabels.cota2x);
		data.cote[pariu::P12] = res->getDouble(dbLabels.cota12);
		data.cote[pariu::PX] = res->getDouble(dbLabels.cotax);
		data.siteId = res->getString(dbLabels.siteId);

		gameData.push_back(data);
	}

	LOGLN("am tras " << gameData.size() << " inregistrari din baza de date.");
}



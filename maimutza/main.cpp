/*
 * main.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#define DUMMY_SQL_SOCKET

#include "../common/configFile.h"
#include "../common/cmdLine.h"
#include "../common/log.h"
#ifdef DUMMY_SQL_SOCKET
#include "../common/DummySQLSock.h"
#else
#include "../common/SQLSock.h"
#endif
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

void faQuery(ISQLSock &sock, std::string const& tabel) {
	auto res = sock.doQuery("select echipa1,echipa2,status_echipe from " + tabel+ " order by data_joc");

	while (res->next()) {
}

int main(int argc, char* argv[]) {
	LOGPREFIX("maimuță");
	std::locale::global(std::locale(""));
	std::cout.imbue(std::locale(""));

	std::map<std::string, std::string> cmdOpts;
	cmdOpts["lPath"] = ".";
	if (!parseCommandLine(argc, argv, cmdOpts, {"table", "config"})) {
		ERROR("Linie de comanda invalida!");
		ERROR("Exemplu linie de comanda valida:\n" <<
				"maimutza --table fotbal --config ~/.maimutza.config --lPath path/to/liste");
		return -1;
	}
	std::map<std::string, std::string> configOpts;
	if (!cmdOpts["config"].empty())
		if (!parseConfigFile(cmdOpts["config"], configOpts, {"dbURI", "dbUser", "dbPassw", "dbName"})) {
			ERROR("maimuța nu poate continua (configurare incompleta)");
			return -1;
		}

	ISQLSock *pSock;
#ifdef DUMMY_SQL_SOCKET
	DummySQLSock dummySqlSock;
	pSock = &dummySqlSock;
#else
	SQLSock sqlSock;
	pSock = &sqlSock;
#endif
	if (!pSock->connect(configOpts["dbURI"], configOpts["dbUser"], configOpts["dbPassw"])
			|| !pSock->setDB(configOpts["dbName"])) {
		ERROR("Nu s-a putut initializa conexiunea la baza de date, shefu'!");
		return -1;
	}

	LOGLN("maimuța incepe loop-ul de cautat echipe netraduse si adaugat in liste...");

	while (true) { // maimuța e nemuritoare!!! muhahaha !!
		faQuery(*sqlSock, cmdOpts["table"]);
		sleep(10); // maimuța doarme 10 secunde intre incercari

		break;
	}

	return 0;
}



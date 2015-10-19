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
#include "../common/dbLabels.h"
#ifdef DUMMY_SQL_SOCKET
#include "../common/DummySQLSock.h"
#else
#include "../common/SQLSock.h"
#endif
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>

// numele argumentelor de pe cmd line:
struct {
	const std::string table {"table"};
	const std::string configPath {"config"};
} cmdLine;

// numele parametrilor din fisierul de config:
struct {
	const std::string dbURI {"URI"};
	const std::string dbUser {"user"};
	const std::string dbPassw {"passw"};
	const std::string dbName {"database"};
} config;

void faQuery(ISQLSock &sock, std::string const& tabel) {
	auto res = sock.doQuery("SELECT "+dbLabels.echipa1+","+dbLabels.echipa2+","+dbLabels.statusTraduceri+" FROM " + tabel+ " ORDER BY data_joc");

	while (res->next()) {
		if (res->getInt(dbLabels.statusTraduceri) == 0)
			continue;

		// trebuie sa mai fie urmatorul meci la aceeasi ora si echipele sa semene ca nume, altfel e o problema

		res->getString(dbLabels.echipa1);
		res->getString(dbLabels.echipa2);
	}
}

int main(int argc, char* argv[]) {
	LOGPREFIX("maimuță");
	std::locale::global(std::locale(""));
	std::cout.imbue(std::locale(""));

	std::map<std::string, std::string> cmdOpts;
	cmdOpts["lPath"] = ".";
	if (!parseCommandLine(argc, argv, cmdOpts, {cmdLine.table, cmdLine.configPath})) {
		ERROR("Linie de comanda invalida!");
		ERROR("Exemplu linie de comanda valida:\n" <<
				"maimutza --table fotbal --config ~/.maimutza.config --lPath path/to/liste");
		return -1;
	}
	std::map<std::string, std::string> configOpts;
	if (!cmdOpts["config"].empty())
		if (!parseConfigFile(cmdOpts["config"], configOpts, {config.dbURI, config.dbUser, config.dbPassw, config.dbName})) {
			ERROR("maimuța nu poate continua (configurare incompleta)");
			return -1;
		}

	ISQLSock *pSock;
#ifdef DUMMY_SQL_SOCKET
	DummySQLSock dummySqlSock("test-maimuța");
	pSock = &dummySqlSock;
#else
	SQLSock sqlSock;
	pSock = &sqlSock;
#endif
	if (!pSock->connect(configOpts[config.dbURI], configOpts[config.dbUser], configOpts[config.dbPassw])
			|| !pSock->setDB(configOpts[config.dbName])) {
		ERROR("Nu s-a putut initializa conexiunea la baza de date, shefu'!");
		return -1;
	}

	LOGLN("maimuța incepe loop-ul de cautat echipe netraduse si adaugat in liste...");

	while (true) { // maimuța e nemuritoare!!! muhahaha !!
		faQuery(*pSock, cmdOpts[cmdLine.table]);
		sleep(10); // maimuța doarme 10 secunde intre incercari

		break;
	}

	return 0;
}



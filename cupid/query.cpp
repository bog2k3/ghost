/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <stdio.h>

void initializeazaDB() {
	try {
	  sql::Driver *driver;
	  sql::Connection *con;
	  sql::Statement *stmt;
	  sql::ResultSet *res;

	  /* Create a connection */
	  driver = get_driver_instance();
	  con = driver->connect("tcp://86.125.49.222:46466", "ghost", "D58jKe3TZJgXaeYTp9DCF4Kx");
	  /* Connect to the MySQL test database */
	  con->setSchema("ghost");

	  stmt = con->createStatement();
	  res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
	  while (res->next()) {
	    std::cout << "\t... MySQL replies: ";
	    /* Access column data by alias or column name */
	    std::cout << res->getString("_message") << std::endl;
	    std::cout << "\t... MySQL says it again: ";
	    /* Access column fata by numeric offset, 1 is the first column */
	    std::cout << res->getString(1) << std::endl;
	  }
	  delete res;
	  delete stmt;
	  delete con;

	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

void faQueryul() {

}



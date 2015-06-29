/*
 * query.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#include <SQLAPI.h>
#include <stdio.h>

void initializeazaDB() {
	SAConnection con; // create connection object

	try
	{
		// connect to database
		// in this example it is Oracle,
		// but can also be Sybase, Informix, DB2
		// SQLServer, InterBase, SQLBase and ODBC
		con.Connect(
			"ghost",     // database name
			"ghost",   // user name
			"D58jKe3TZJgXaeYTp9DCF4Kx",   // password
			SA_Oracle_Client);

		printf("We are connected!\n");

		// Disconnect is optional
		// autodisconnect will ocur in destructor if needed
		con.Disconnect();

		printf("We are disconnected!\n");
	}
	catch(SAException &x)
	{
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		try
		{
			// on error rollback changes
			con.Rollback();
		}
		catch(SAException &)
		{
		}
		// print error message
		printf("%s\n", (const char*)x.ErrText());
	}
}

void faQueryul() {

}



/*
 * DummySQLSock.h
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#ifndef DUMMYSQLSOCK_H_
#define DUMMYSQLSOCK_H_

#include "ISQLSock.h"
#include <vector>
#include <string>
#include <map>

class DummySQLSock: public ISQLSock {
public:
	DummySQLSock(const char* filePath);
	virtual ~DummySQLSock() override;

	virtual bool connect(std::string const& URI, std::string const& user, std::string const& passw) override;
	virtual bool setDB(std::string const& databaseName) override;

	virtual std::unique_ptr<sql::ResultSet> doQuery(std::string const& query) override;

private:
	int nRecords_ = 0;
	int nColoane_ = 0;
	std::vector<std::vector<std::string>> coloane_;
	std::vector<std::string> numeColoane_;
	std::map<std::string, int> nume2Index_;

	void insert(std::vector<std::string> const& val);

	std::unique_ptr<sql::ResultSet> doSelect(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doInsert(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doUpdate(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doDelete(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
};

#endif /* DUMMYSQLSOCK_H_ */

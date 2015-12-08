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
	int nColoane_ = 0;
	std::vector<std::vector<std::string>> records_;
	std::vector<std::string> numeColoane_;
	std::map<std::string, int> nume2Index_;

	void insert(std::vector<std::string> const& val);

	std::unique_ptr<sql::ResultSet> doSelect(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doInsert(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doUpdate(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
	std::unique_ptr<sql::ResultSet> doDelete(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok);
};

class WhereClause {
public:
	bool parse(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned &crtTok);
	bool validate(std::vector<std::string> const& record, std::map<std::string, int> const& name2Index);

private:
	bool equalClause_ = true;
	std::string column_;
	std::string value_;
};

#endif /* DUMMYSQLSOCK_H_ */

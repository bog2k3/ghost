/*
 * DummySQLSock.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#include "DummySQLSock.h"
#include "DummySQLResult.h"
#include "../common/strManip.h"
#include "../common/log.h"
#include <fstream>
#include <set>

DummySQLSock::DummySQLSock(const char* filePath) {
	LOGPREFIX("DummySQLSock");
	std::ifstream f(filePath);
	if (!f.is_open()) {
		ERROR("nu s-a putut deschide fisierul baza de date \"" << filePath << "\"");
		return;
	}
	std::string cnames;
	std::getline(f, cnames);
	auto labels = strSplit(cnames,';');
	nColoane_ = labels.size();
	for (int i=0; i<nColoane_; i++) {
		indexColoane_[i] = labels[i];
		strUpper(labels[i]);	// column names will be uppercase
		coloane_[labels[i]];
	}
	std::string l;
	while (std::getline(f, l)) {
		if (l.empty())
			continue;
		auto valori = strSplit(l, ';');
		insert(valori);
	}
}

DummySQLSock::~DummySQLSock() {
	// TODO Auto-generated destructor stub
}

void DummySQLSock::insert(std::vector<std::string> const& val) {
	for (int i=0; i<nColoane_; i++)
		coloane_[indexColoane_[i]].push_back(i < val.size() ? val[i] : "");
}

bool DummySQLSock::connect(std::string const& URI, std::string const& user, std::string const& passw) {
	return true;
}

bool DummySQLSock::setDB(std::string const& databaseName) {
	return true;
}

enum class SQL_CMD {
	SELECT,
	INSERT,
	UPDATE,
	DELETE,

	INVALID
};

enum class SQL_VECTOR {
	FROM,
	INTO,

	INVALID
};

enum class SQL_SORTDIR {
	ASCENDING,
	DESCENDING,

	INVALID
};

SQL_CMD getSQLCmd(std::string const& strCmd) {
	auto upper = strCmd;
	strUpper(upper);
	if (upper == "SELECT")
		return SQL_CMD::SELECT;
	if (upper == "INSERT")
		return SQL_CMD::INSERT;
	if (upper == "UPDATE")
		return SQL_CMD::UPDATE;
	if (upper == "DELETE")
		return SQL_CMD::DELETE;
	return SQL_CMD::INVALID;
}

std::unique_ptr<sql::ResultSet> DummySQLSock::doSelect(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok) {
	// we expect a query of this form:
	// SELECT <*>/<column-names,> FROM <table-name> {optional [WHERE] <expr>} {optional [ORDER BY] <column-name> [DIRECTION]}
	// <expr> is a combination of AND/OR of atoms; [NO parantheses supported]
	// an atom has this form: <column-name> [OPERATOR] <value>
	// [OPERATOR] =,!=,<,>,<=,>=
	unsigned fromPos = crtTok;
	while (fromPos < upperTokens.size() && upperTokens[fromPos] != "FROM")
		fromPos++;
	if (fromPos == crtTok) {
		ERROR("invalid query: missing column names before FROM");
		return nullptr;
	}
	if (fromPos == upperTokens.size()) {
		ERROR("invalid query: missing FROM");
		return nullptr;
	}
	std::set<std::string> columns;
	if (tokens[crtTok] == "*") {
		// select all columns
		for (auto pair : coloane_)
			columns.insert(pair.first);
		crtTok++;
	} else while (crtTok < fromPos) {
		// select columns listed in comma separated format
		std::vector<std::string> cols = strSplit(upperTokens[crtTok], ',');
		for (auto c : cols) {
			if (coloane_.find(c) == coloane_.end()) {
				ERROR("invalid query: unknown column " << c);
			}
			columns.insert(c);
		}
		crtTok++;
	}
	// pass FROM:
	crtTok++;
	if (crtTok == tokens.size()) {
		ERROR("invalid query: missing table name after FROM");
		return nullptr;
	}
	std::string tableName = upperTokens[crtTok++];
	// we ignore the tableName anyway

	bool order = false;
	bool order_descending = false;
	// WhereClause where;
	if (crtTok < tokens.size()) {
		// we have extra stuff like WHERE or ORDER
	}
//	std::unique_ptr<DummyResultSet> pRes(new DummyResultSet());
	for (int i=0; i<nRecords_; i++) {
		bool recordValid = true;// = where.pass(i);
		if (recordValid) {
			// add this record's values to the output
		}
	}
}

std::unique_ptr<sql::ResultSet> DummySQLSock::doInsert(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok) {

}

std::unique_ptr<sql::ResultSet> DummySQLSock::doUpdate(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok) {

}

std::unique_ptr<sql::ResultSet> DummySQLSock::doDelete(std::vector<std::string> const& upperTokens, std::vector<std::string> const& tokens, unsigned crtTok) {

}

std::unique_ptr<sql::ResultSet> DummySQLSock::doQuery(std::string const& query) {
	LOGPREFIX("DummySQLSock");
	std::string q = query;
	// here goes dragons - parse SQL syntax :-)
	/*
	 * we support only simple queries built by the following pattern:
	 * [COMMAND] <column-names-,> [VECTOR] <table-name> {optional [VALUES] (val1,val2,...)} {optional [WHERE] [condition]} {optional [ORDER BY] <column-name> [DIRECTION]} ;
	 *
	 * now each field may have a limited set of values:
	 * [COMMAND] "SELECT" / "INSERT" / "DELETE" / "UPDATE"
	 * [VECTOR] "FROM" / "INTO" / "" - must be valid with respect to [COMMAND]
	 * [condition] <column-name> [=,!=] <value> {optional [AND]/[OR] <expr>}
	 * [DIRECTION] "ASCENDING" / "DESCENDING"
	 */

	if (q.back() == ';')
		q.pop_back();
	std::vector<std::string> tokens = strSplit(q, ' ');
	std::vector<std::string> upperTokens = tokens;
	for (auto s : upperTokens)
		strUpper(s);
	unsigned crtTok = 0;
	switch (getSQLCmd(tokens[crtTok++])) {
	case SQL_CMD::SELECT:
		return doSelect(upperTokens, tokens, crtTok);
		break;
	case SQL_CMD::INSERT:
		return doInsert(upperTokens, tokens, crtTok);
		break;
	case SQL_CMD::UPDATE:
		return doUpdate(upperTokens, tokens, crtTok);
		break;
	case SQL_CMD::DELETE:
		return doDelete(upperTokens, tokens, crtTok);
		break;
	default:
		ERROR("Invalid SQL command: " + tokens[crtTok-1]);
		return nullptr;
	}
}

/*
 * DummySQLResult.h
 *
 *  Created on: Dec 2, 2015
 *      Author: bog
 */

#ifndef DUMMYSQLRESULT_H_
#define DUMMYSQLRESULT_H_

#include "assert.h"

#include <cppconn/resultset.h>
#include <map>
#include <vector>
#include <string>


#define NOTIMPLEMENTED { assertDbg(!"NOT IMPLEMENTED!!!"); }

class DummyResultSet : public sql::ResultSet
{
public:

	DummyResultSet(std::map<std::string, std::vector<std::string>> coloane,
			std::vector<std::string> ordinea);

	virtual ~DummyResultSet() override {}

	virtual bool absolute(int row) override NOTIMPLEMENTED

	virtual void afterLast() override;

	virtual void beforeFirst() override;

	virtual void cancelRowUpdates() override NOTIMPLEMENTED

	virtual void clearWarnings() override NOTIMPLEMENTED

	virtual void close() override NOTIMPLEMENTED

	virtual uint32_t findColumn(const sql::SQLString& columnLabel) const override;

	virtual bool first() override;

	virtual std::istream * getBlob(uint32_t columnIndex)  const override NOTIMPLEMENTED
	virtual std::istream * getBlob(const sql::SQLString& columnLabel) const override NOTIMPLEMENTED

	virtual bool getBoolean(uint32_t columnIndex) const override;
	virtual bool getBoolean(const sql::SQLString& columnLabel) const override;

	virtual int getConcurrency() override NOTIMPLEMENTED
	virtual sql::SQLString getCursorName() override NOTIMPLEMENTED

	virtual long double getDouble(uint32_t columnIndex) const override;
	virtual long double getDouble(const sql::SQLString& columnLabel) const override;

	virtual int getFetchDirection() override NOTIMPLEMENTED
	virtual size_t getFetchSize() override NOTIMPLEMENTED
	virtual int getHoldability() override NOTIMPLEMENTED

	virtual int32_t getInt(uint32_t columnIndex) const override;
	virtual int32_t getInt(const sql::SQLString& columnLabel) const override;

	virtual uint32_t getUInt(uint32_t columnIndex) const override;
	virtual uint32_t getUInt(const sql::SQLString& columnLabel) const override;

	virtual int64_t getInt64(uint32_t columnIndex) const override;
	virtual int64_t getInt64(const sql::SQLString& columnLabel) const override;

	virtual uint64_t getUInt64(uint32_t columnIndex) const override;
	virtual uint64_t getUInt64(const sql::SQLString& columnLabel) const override;

	virtual sql::ResultSetMetaData * getMetaData() const override NOTIMPLEMENTED

	virtual size_t getRow() const override;

	virtual sql::RowID * getRowId(uint32_t columnIndex) override NOTIMPLEMENTED
	virtual sql::RowID * getRowId(const sql::SQLString & columnLabel) override NOTIMPLEMENTED

	virtual const sql::Statement * getStatement() const override NOTIMPLEMENTED

	virtual sql::SQLString getString(uint32_t columnIndex)  const override;
	virtual sql::SQLString getString(const sql::SQLString& columnLabel) const override;

	virtual enum_type getType() const override NOTIMPLEMENTED

	virtual void getWarnings() override NOTIMPLEMENTED

	virtual void insertRow() override NOTIMPLEMENTED

	virtual bool isAfterLast() const override;

	virtual bool isBeforeFirst() const override;

	virtual bool isClosed() const override NOTIMPLEMENTED;

	virtual bool isFirst() const override;

	virtual bool isLast() const override;

	virtual bool isNull(uint32_t columnIndex) const override NOTIMPLEMENTED
	virtual bool isNull(const sql::SQLString& columnLabel) const override NOTIMPLEMENTED

	virtual bool last() override;

	virtual bool next() override;

	virtual void moveToCurrentRow() override NOTIMPLEMENTED

	virtual void moveToInsertRow() override NOTIMPLEMENTED

	virtual bool previous() override;

	virtual void refreshRow() override NOTIMPLEMENTED

	virtual bool relative(int rows) override NOTIMPLEMENTED

	virtual bool rowDeleted() override NOTIMPLEMENTED

	virtual bool rowInserted() override NOTIMPLEMENTED

	virtual bool rowUpdated() override NOTIMPLEMENTED

	virtual void setFetchSize(size_t rows) override NOTIMPLEMENTED

	virtual size_t rowsCount() const override;

	virtual bool wasNull() const override NOTIMPLEMENTED

private:
	int nRecords_;
	int current_;

	std::map<std::string, std::vector<std::string>> coloane_;
	std::vector<std::string> ordinea_;

	template<typename T, T valDefault>
	T strToVal(std::string const& str) const;

	template<typename T, T valDefault>
	T getValue(uint32_t columnIndex) const;
};



#endif /* DUMMYSQLRESULT_H_ */

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

	DummyResultSet(std::vector<std::vector<std::string>> const& rows,
			std::vector<std::string> const& numeColoane);

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

	virtual size_t getRow() const override NOTIMPLEMENTED

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
	int nRecords_ = 0;
	int current_ = -1;

	static constexpr unsigned long NOCOLUMN = 0xffffffff;

	std::vector<std::vector<std::string>> rows_;
	std::vector<std::string> numeColoane_;

	template<typename T>
	T strToVal(std::string const& str, T const& valDefault) const;

	template<typename T>
	T getValue(uint32_t columnIndex, T const& valDefault) const;

	template<typename T>
	T getValue(std::string const& label, T const& valDefault) const;
};



#endif /* DUMMYSQLRESULT_H_ */

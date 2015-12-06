/*
 * DummySQLResult.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: bog
 */

#include "DummySQLResult.h"
#include "strManip.h"

#include <sstream>

void DummyResultSet::afterLast() {
	current_ = nRecords_;
}

void DummyResultSet::beforeFirst() {
	current_ = -1;
}

bool DummyResultSet::isAfterLast() const {
	return current_ == nRecords_;
}

bool DummyResultSet::isBeforeFirst() const {
	return current_ == -1;
}

bool DummyResultSet::isFirst() const {
	return current_ == 0;
}

bool DummyResultSet::isLast() const {
	return current_ == nRecords_-1;
}

bool DummyResultSet::last() {
	current_ = nRecords_ - 1;
	return current_ >= 0;
}

bool DummyResultSet::next() {
	if (current_ < nRecords_)
		current_++;
	return current_ < nRecords_;
}

bool DummyResultSet::previous() {
	if (current_ >= 0)
		current_--;
	return current_>=0;
}

bool DummyResultSet::first() {
	if (nRecords_ > 0)
		current_ = 0;
	else
		current_ = -1;
	return current_ == 0;
}

size_t DummyResultSet::rowsCount() const {
	return nRecords_;
}

uint32_t DummyResultSet::findColumn(const sql::SQLString& columnLabel) const {
	std::string upper = strUpper(columnLabel);
	for (uint i=0; i<numeColoane_.size(); i++)
		if (upper == numeColoane_[i])
			return i;
	return NOCOLUMN;
}

template<>
bool DummyResultSet::strToVal<bool>(std::string const& str, bool const& valDefault) const {
	std::string su = strUpper(str);
	return su == "TRUE";
}

template<>
std::string DummyResultSet::strToVal<std::string>(std::string const& str, std::string const& valDefault) const {
	return str.empty() ? valDefault : str;
}

template<typename T>
T DummyResultSet::strToVal(std::string const& str, T const& valDefault) const {
	std::stringstream ss(str);
	T value;
	ss >> value;
	if (ss.fail())
		return valDefault;
	return value;
}

template<typename T>
T DummyResultSet::getValue(uint32_t columnIndex, T const& valDefault) const {
	if (columnIndex >= coloane_.size())
		return valDefault;
	auto value = coloane_[columnIndex][current_];
	return strToVal<T>(value, valDefault);
}

template<typename T>
T DummyResultSet::getValue(std::string const& label, T const& valDefault) const {
	auto idx = findColumn(label);
	if (idx == NOCOLUMN)
		return valDefault;
	return getValue<T>(idx, valDefault);
}

bool DummyResultSet::getBoolean(uint32_t columnIndex) const {
	return getValue<bool>(columnIndex, false);
}

bool DummyResultSet::getBoolean(const sql::SQLString& columnLabel) const {
	return getValue<bool>(columnLabel, false);
}

long double DummyResultSet::getDouble(uint32_t columnIndex) const {
	return getValue<long double>(columnIndex, 0);
}

long double DummyResultSet::getDouble(const sql::SQLString& columnLabel) const {
	return getValue<long double>(columnLabel, 0);
}

int32_t DummyResultSet::getInt(uint32_t columnIndex) const {
	return getValue<int32_t>(columnIndex, 0);
}

int32_t DummyResultSet::getInt(const sql::SQLString& columnLabel) const {
	return getValue<int32_t>(columnLabel, 0);
}

uint32_t DummyResultSet::getUInt(uint32_t columnIndex) const {
	return getValue<uint32_t>(columnIndex, 0);
}

uint32_t DummyResultSet::getUInt(const sql::SQLString& columnLabel) const {
	return getValue<uint32_t>(columnLabel, 0);
}

int64_t DummyResultSet::getInt64(uint32_t columnIndex) const {
	return getValue<int64_t>(columnIndex, 0);
}

int64_t DummyResultSet::getInt64(const sql::SQLString& columnLabel) const {
	return getValue<int64_t>(columnLabel, 0);
}

uint64_t DummyResultSet::getUInt64(uint32_t columnIndex) const {
	return getValue<uint64_t>(columnIndex, 0);
}

uint64_t DummyResultSet::getUInt64(const sql::SQLString& columnLabel) const {
	return getValue<uint64_t>(columnLabel, 0);
}

sql::SQLString DummyResultSet::getString(uint32_t columnIndex)  const {
	return getValue<std::string>(columnIndex, "");
}

sql::SQLString DummyResultSet::getString(const sql::SQLString& columnLabel) const {
	return getValue<std::string>(columnLabel, "");
}

DummyResultSet::DummyResultSet(std::vector<std::vector<std::string>> const& coloane,
		std::vector<std::string> const& numeColoane)
: coloane_(coloane)
, numeColoane_(numeColoane)
{
	nRecords_ = coloane[0].size();
	for (uint i=1; i<coloane.size(); i++) {
		assertDbg(coloane[i].size() == nRecords_ && "Toate coloanele trebuie sa aiba acelasi numar de inregistrari!");
	}
	for (auto &c : numeColoane_)
		c = strUpper(c);
}

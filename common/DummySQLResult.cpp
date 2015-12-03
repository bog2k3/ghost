/*
 * DummySQLResult.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: bog
 */

#include "DummySQLResult.h"
#include "strManip.h"

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
	std::string upper = columnLabel;
	strUpper(upper);
	for (int i=0; i<ordinea_.size(); i++)
		if (upper == ordinea_[i])
			return i;
	return 0xffffffff;
}

template<>
bool DummyResultSet::strToVal<bool, false>(std::string const& str) const {
	return str == "TRUE";
}

template<typename T, T valDefault>
T DummyResultSet::getValue(uint32_t columnIndex) const {
	if (columnIndex >= coloane_.size())
		return valDefault;
	auto const &col = coloane_.find(ordinea_[columnIndex]);
	if (col == coloane_.end())
		return valDefault;
	auto value = col->second[current_];
	strUpper(value);
	return strToVal<T, valDefault>(value);
}

bool DummyResultSet::getBoolean(uint32_t columnIndex) const {
	return getValue<bool, false>(columnIndex);
}

bool DummyResultSet::getBoolean(const sql::SQLString& columnLabel) const {

}

long double DummyResultSet::getDouble(uint32_t columnIndex) const {

}

long double DummyResultSet::getDouble(const sql::SQLString& columnLabel) const {

}

int32_t DummyResultSet::getInt(uint32_t columnIndex) const {

}

int32_t DummyResultSet::getInt(const sql::SQLString& columnLabel) const {

}

uint32_t DummyResultSet::getUInt(uint32_t columnIndex) const {

}

uint32_t DummyResultSet::getUInt(const sql::SQLString& columnLabel) const {

}

int64_t DummyResultSet::getInt64(uint32_t columnIndex) const {

}

int64_t DummyResultSet::getInt64(const sql::SQLString& columnLabel) const {

}

uint64_t DummyResultSet::getUInt64(uint32_t columnIndex) const {

}

uint64_t DummyResultSet::getUInt64(const sql::SQLString& columnLabel) const {

}

size_t DummyResultSet::getRow() const {

}

sql::SQLString DummyResultSet::getString(uint32_t columnIndex)  const {

}

sql::SQLString DummyResultSet::getString(const sql::SQLString& columnLabel) const {

}

DummyResultSet::DummyResultSet(std::map<std::string, std::vector<std::string>> coloane,
			std::vector<std::string> ordinea)
: coloane_(coloane)
, ordinea_(ordinea)
{
	auto it = coloane_.begin();
	nRecords_ = it->second.size();
	while (it != coloane_.end()) {
		assertDbg(it->second.size() == nRecords_ && "Toate coloanele trebuie sa aiba acelasi numar de inregistrari!");
		it++;
	}
	for (auto &c : ordinea_)
		strUpper(c);
}

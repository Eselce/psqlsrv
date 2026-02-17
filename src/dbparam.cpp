
#include <cstring>

#include "dbparam.hpp"

DBparameter::DBparameter(const int nParams)
{
	m_nParams = nParams;

	m_types = new DBparameterType[nParams];
	m_values = new const char *[nParams];
	m_lengths = new int[nParams];
	m_formats = new DBparameterFormat[nParams];
}

DBparameter::~DBparameter(void)
{
	delete[] m_types;
	delete[] m_values;
	delete[] m_lengths;
	delete[] m_formats;
}

void DBparameter::resize(const int nParams)
{
	m_nParams = nParams;

	delete[] m_types;
	delete[] m_values;
	delete[] m_lengths;
	delete[] m_formats;

	m_types = new DBparameterType[nParams];
	m_values = new const char *[nParams];
	m_lengths = new int[nParams];
	m_formats = new DBparameterFormat[nParams];
}

void DBparameter::setany(const void *value, const int pos, const DBparameterType type, const int length, const DBparameterFormat format)
{
	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	m_types[i] = type;
	m_values[i] = static_cast<const char *>(value);
	m_lengths[i] = length;
	m_formats[i] = format;
}

void DBparameter::set(const int &value, const int pos)
{
	this->setany(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::set(const short int &value, const int pos)
{
	this->setany(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::set(const long int &value, const int pos)
{
	this->setany(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::set(const float &value, const int pos)
{
	this->setany(&value, pos, 4, sizeof(float), FORMAT_BINARY);
}

void DBparameter::set(const double &value, const int pos)
{
	this->setany(&value, pos, 5, sizeof(double), FORMAT_BINARY);
}

void DBparameter::set(const std::string &value, const int pos)
{
	this->set(value.c_str(), pos);
}

void DBparameter::set(const char *value, const int pos)
{
	this->setany(value, pos, 6, std::strlen(value), FORMAT_TEXT);
}

int DBparameter::count(void) const
{
	return m_nParams;
}

const DBparameterType *DBparameter::types(void) const
{
	return m_types;
}

const char * const *DBparameter::values(void) const
{
	return m_values;
}

const int *DBparameter::lengths(void) const
{
	return m_lengths;
}

const int *DBparameter::formats(void) const
{
	// This is a bit hacky, but it allows us to return the formats as an array of ints, which is what the DBparameterFormat enum is.
	return const_cast<const int *>(reinterpret_cast<int *>(m_formats));
}


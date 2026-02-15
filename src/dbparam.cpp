
#include <cstring>

#include "dbparam.hpp"

DBparameter::DBparameter(const int nParams)
{
	m_nParams = nParams;

	m_types = new DBtype[nParams];
	m_values = new const char *[nParams];
	m_lengths = new int[nParams];
	m_formats = new int[nParams];
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

	m_types = new DBtype[nParams];
	m_values = new const char *[nParams];
	m_lengths = new int[nParams];
	m_formats = new int[nParams];
}

void DBparameter::setany(const void *value, const int pos, const DBtype type, const int length, const int format)
{
	m_types[pos] = type;
	m_values[pos] = static_cast<const char *>(value);
	m_lengths[pos] = length;
	m_formats[pos] = format;
}

void DBparameter::set(const int &value, const int pos)
{
	this->setany(&value, pos, 1, sizeof(int), 0);
}

void DBparameter::set(const short int &value, const int pos)
{
	this->setany(&value, pos, 2, sizeof(short int), 0);
}

void DBparameter::set(const long int &value, const int pos)
{
	this->setany(&value, pos, 3, sizeof(long int), 0);
}

void DBparameter::set(const float &value, const int pos)
{
	this->setany(&value, pos, 4, sizeof(float), 0);
}

void DBparameter::set(const double &value, const int pos)
{
	this->setany(&value, pos, 5, sizeof(double), 0);
}

void DBparameter::set(const std::string &value, const int pos)
{
	this->set(value.c_str(), pos);
}

void DBparameter::set(const char *value, const int pos)
{
	this->setany(value, pos, 6, std::strlen(value), 0);
}

int DBparameter::count(void) const
{
	return m_nParams;
}

const DBtype *DBparameter::types(void) const
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
	return m_formats;
}


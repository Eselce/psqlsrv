
#include <iostream>

#include <cstring>

#include "dbtupel.hpp"

DBtupel::DBtupel(const int nFields)
{
	m_nFields = nFields;

	m_types = new DBparameterType[nFields];
	m_values = new const char *[nFields];
	m_lengths = new int[nFields];
	m_formats = new DBparameterFormat[nFields];
}

DBtupel::~DBtupel(void)
{
	delete[] m_types;
	delete[] m_values;
	delete[] m_lengths;
	delete[] m_formats;

#if defined(_DEBUG)
	std::clog << "Deleted tupel arrays[" << m_nFields << "]: " << this << std::endl;
#endif
}

void DBtupel::resize(const int nFields)
{
	m_nFields = nFields;

	delete[] m_types;
	delete[] m_values;
	delete[] m_lengths;
	delete[] m_formats;

#if defined(_DEBUG)
	std::clog << "Deleted old tupel arrays: " << this << std::endl;
#endif

	m_types = new DBparameterType[nFields];
	m_values = new const char *[nFields];
	m_lengths = new int[nFields];
	m_formats = new DBparameterFormat[nFields];

#if defined(_DEBUG)
	std::clog << "Resized tupel to " << nFields << " fields: " << this << std::endl;
#endif
}

void DBtupel::bindany(const void *value, const int pos, const DBparameterType type, const int length, const DBparameterFormat format)
{
	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	m_types[i] = type;
	m_values[i] = static_cast<const char *>(value);
	m_lengths[i] = length;
	m_formats[i] = format;
}

void DBtupel::bind(const int &value, const int pos)
{
	this->bindany(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBtupel::bind(const short int &value, const int pos)
{
	this->bindany(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBtupel::bind(const long int &value, const int pos)
{
	this->bindany(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBtupel::bind(const float &value, const int pos)
{
	this->bindany(&value, pos, 4, sizeof(float), FORMAT_BINARY);
}

void DBtupel::bind(const double &value, const int pos)
{
	this->bindany(&value, pos, 5, sizeof(double), FORMAT_BINARY);
}

void DBtupel::bind(const std::string &value, const int pos)
{
	this->bind(value.c_str(), pos);
}

void DBtupel::bind(const char *value, const int pos)
{
	this->bindany(value, pos, 6, std::strlen(value), FORMAT_TEXT);
}

int DBtupel::count(void) const
{
	return m_nFields;
}

const DBparameterType *DBtupel::types(void) const
{
	return m_types;
}

const char **DBtupel::values(void) const
{
	return m_values;
}

const int *DBtupel::lengths(void) const
{
	return m_lengths;
}

const int *DBtupel::formats(void) const
{
	// This is a bit hacky, but it allows us to return the formats as an array of ints, which is what the DBparameterFormat enum is.
	return const_cast<const int *>(reinterpret_cast<int *>(m_formats));
}


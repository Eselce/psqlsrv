
#include <cstring>

#include "pg_type.h"

#include "pgtupel.hpp"

PGtupel::PGtupel(const int nFields)
:	DBtupel(nFields)
{
}

PGtupel::~PGtupel(void)
{
}

void PGtupel::bindany(const void *value, const int pos, const Oid type, const int length, const DBparameterFormat format)
{
	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	m_types[i] = type;
	m_values[i] = static_cast<const char *>(value);
	m_lengths[i] = length;
	m_formats[i] = format;
}

void PGtupel::bind(const int &value, const int pos)
{
	this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGtupel::bind(const short int &value, const int pos)
{
	this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGtupel::bind(const long int &value, const int pos)
{
	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGtupel::bind(const float &value, const int pos)
{
	this->bindany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
}

void PGtupel::bind(const double &value, const int pos)
{
	this->bindany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
}

void PGtupel::bind(const std::string &value, const int pos)
{
	this->bind(value.c_str(), pos);
}

void PGtupel::bind(const char *value, const int pos)
{
	this->bindany(value, pos, VARCHAROID, std::strlen(value), FORMAT_TEXT);
}

const Oid *PGtupel::types(void) const
{
	return static_cast<const Oid *>(m_types);
}


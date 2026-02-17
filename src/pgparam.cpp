
#include <cstring>

#include "pg_type.h"

#include "pgparam.hpp"

PGparameter::PGparameter(const int nParams)
:	DBparameter(nParams)
{
}

PGparameter::~PGparameter(void)
{
}

void PGparameter::setany(const void *value, const int pos, const Oid type, const int length, const DBparameterFormat format)
{
	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	m_types[i] = type;
	m_values[i] = static_cast<const char *>(value);
	m_lengths[i] = length;
	m_formats[i] = format;
}

void PGparameter::set(const int &value, const int pos)
{
	this->setany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGparameter::set(const short int &value, const int pos)
{
	this->setany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGparameter::set(const long int &value, const int pos)
{
	this->setany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGparameter::set(const float &value, const int pos)
{
	this->setany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
}

void PGparameter::set(const double &value, const int pos)
{
	this->setany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
}

void PGparameter::set(const std::string &value, const int pos)
{
	this->set(value.c_str(), pos);
}

void PGparameter::set(const char *value, const int pos)
{
	this->setany(value, pos, VARCHAROID, std::strlen(value), FORMAT_TEXT);
}

const Oid *PGparameter::types(void) const
{
	return static_cast<const Oid *>(m_types);
}


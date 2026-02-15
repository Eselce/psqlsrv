
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

void PGparameter::setany(const void *value, const int pos, const Oid type, const int length, const int format)
{
	m_types[pos] = static_cast<DBtype>(type);
	m_values[pos] = static_cast<const char *>(value);
	m_lengths[pos] = length;
	m_formats[pos] = format;
}

void PGparameter::set(const int &value, const int pos)
{
	this->setany(&value, pos, INT4OID, sizeof(int), 0);
}

void PGparameter::set(const short int &value, const int pos)
{
	this->setany(&value, pos, INT2OID, sizeof(short int), 0);
}

void PGparameter::set(const long int &value, const int pos)
{
	this->setany(&value, pos, INT8OID, sizeof(long int), 0);
}

void PGparameter::set(const float &value, const int pos)
{
	this->setany(&value, pos, FLOATOID, sizeof(float), 0);
}

void PGparameter::set(const double &value, const int pos)
{
	this->setany(&value, pos, DOUBLEOID, sizeof(double), 0);
}

void PGparameter::set(const std::string &value, const int pos)
{
	this->set(value.c_str(), pos);
}

void PGparameter::set(const char *value, const int pos)
{
	this->setany(value, pos, VARCHAROID, std::strlen(value), 0);
}

const Oid *PGparameter::types(void) const
{
	return static_cast<const Oid *>(m_types);
}


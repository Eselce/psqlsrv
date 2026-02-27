
#include <iostream>

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

void PGparameter::bindany(const void *value, const int pos, const Oid type, const int length, const DBparameterFormat format)
{
	if ((pos < 1) || (pos > this->m_nParams)) {
		std::cerr << "Error in bindany(): Illegal index " << pos << "! Should be between 1 and " << this->m_nParams << "..." << std::endl;

		return;
	}

	if (format == FORMAT_BINARY) {
		// libpq handles binary data with big endian!
		value = this->convertbigendian(value, length, pos);
	}

	DBparameter::bindany(value, pos, type, length, format);
}

void PGparameter::bind(const int &value, const int pos)
{
	this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGparameter::bind(const short int &value, const int pos)
{
	this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGparameter::bind(const long int &value, const int pos)
{
	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGparameter::bind(const float &value, const int pos)
{
	this->bindany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
}

void PGparameter::bind(const double &value, const int pos)
{
	this->bindany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
}

void PGparameter::bind(const std::string &value, const int pos)
{
	this->bind(value.c_str(), pos);
}

void PGparameter::bind(const char *value, const int pos)
{
	this->bindany(value, pos, VARCHAROID, std::strlen(value), FORMAT_TEXT);
}

const Oid *PGparameter::types(void) const
{
	return static_cast<const Oid *>(this->m_types);
}


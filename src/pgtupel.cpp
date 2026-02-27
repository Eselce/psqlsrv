
#include <iostream>

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
	if ((pos < 1) || (pos > this->m_nFields)) {
		std::cerr << "Error in bindany(): Illegal index " << pos << "! Should be between 1 and " << this->m_nFields << "..." << std::endl;

		return;
	}

	if (format == FORMAT_BINARY) {
		// libpq handles binary data with big endian!
		value = this->convertlittleendian(value, length, pos);
	}

	DBtupel::bindany(value, pos, type, length, format);
}

void PGtupel::bindvar(const int &value, const int pos)
{
	this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGtupel::bindvar(const short int &value, const int pos)
{
	this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGtupel::bindvar(const long int &value, const int pos)
{
	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGtupel::bindvar(const float &value, const int pos)
{
	this->bindany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
}

void PGtupel::bindvar(const double &value, const int pos)
{
	this->bindany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
}

void PGtupel::bindvar(const std::string &value, const int pos)
{
	this->bindvar(value.c_str(), pos);
}

void PGtupel::bindvar(const char *value, const int pos)
{
	this->bindany(value, pos, VARCHAROID, std::strlen(value), FORMAT_TEXT);
}

const Oid *PGtupel::types(void) const
{
	return static_cast<const Oid *>(this->m_types);
}


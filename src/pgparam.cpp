
#include <iostream>

#include <cstring>

#include "pg_type.h"

#include "pgparam.hpp"

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

void PGparameter::bindvar(const signed int &value, const int pos)
{
	this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGparameter::bindvar(const unsigned int &value, const int pos)
{
	this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
}

void PGparameter::bindvar(const signed short int &value, const int pos)
{
	this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGparameter::bindvar(const unsigned short int &value, const int pos)
{
	this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
}

void PGparameter::bindvar(const signed long int &value, const int pos)
{
	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGparameter::bindvar(const unsigned long int &value, const int pos)
{
	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGparameter::bindvar(const float &value, const int pos)
{
	this->bindany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
}

void PGparameter::bindvar(const double &value, const int pos)
{
	this->bindany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
}

void PGparameter::bindvar(const std::string &value, const int pos)
{
	this->bindvar(value.c_str(), pos);
}

void PGparameter::bindvar(const char *value, const int pos)
{
	this->bindany(value, pos, VARCHAROID, std::strlen(value), FORMAT_TEXT);
}

const Oid *PGparameter::types(void) const
{
	return static_cast<const Oid *>(this->m_types);
}

std::string PGparameter::to_string(void) const
{
	std::string ret = std::string("[") + std::to_string(this->m_nParams) + "](";

	for (int i = 0; i < this->m_nParams; ++i) {
		DBparameterType type = this->m_types[i];
		const char *value = this->m_values[i];
		const short int val2 = be16toh(*reinterpret_cast<const short int *>(value));
		const int val4 = be32toh(*reinterpret_cast<const int *>(value));
		const long int val8 = be64toh(*reinterpret_cast<const long int *>(value));

		if (i != 0) {
			ret += ", ";
		}
		switch (type) {
		case INT4OID:	ret += std::to_string(static_cast<int>(val4)); break;
		case INT2OID:	ret += std::to_string(static_cast<short int>(val2)) + "h"; break;
		case INT8OID:	ret += std::to_string(static_cast<long int>(val8)) + "l"; break;
		case FLOATOID:	ret += std::to_string(*reinterpret_cast<const float *>(&val4)) + "f"; break;
		case DOUBLEOID:	ret += std::to_string(*reinterpret_cast<const double *>(&val8)) + "Lf"; break;
		case VARCHAROID:ret = ret + "\"" + value + "\""; break;
		default:		ret = ret + "#" + std::to_string(type) + "#"; break;
		}
	}
	ret += ")";

	return ret;
}


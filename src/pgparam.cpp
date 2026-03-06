
#include <iostream>

#include <cstring>

#include "pg_type.h"

#include "pgparam.hpp"

PGparameter::~PGparameter(void)
{
}

int PGparameter::parse(const char *str, const char delim)
{
	int p = 0;

	if (str == nullptr) {
		std::cerr << "PGparameter::parse(): Parsing null string!" << std::endl;

		return p;
	}

	for (int i = 0, pos = i + 1; i < this->m_nParams; ++i, ++pos) {
		DBparameterType type = this->m_types[i];
		const char *valuechar = (str + p);
		const parameter *value = reinterpret_cast<const parameter *>(valuechar);
		int len;

		switch (type) {
		case INT4OID:	len = this->parsevar(value->m_int, pos, valuechar); break;
		case INT2OID:	len = this->parsevar(value->m_short, pos, valuechar); break;
		case INT8OID:	len = this->parsevar(value->m_long, pos, valuechar); break;
		case FLOATOID:	len = this->parsevar(value->m_float, pos, valuechar); break;
		case DOUBLEOID:	len = this->parsevar(value->m_double, pos, valuechar); break;
		case VARCHAROID:len = this->parsevar(valuechar, pos, valuechar); break;
		default:		len = 0; std::cerr << "#" << std::to_string(type) << "#" << std::endl; break;
		}

		const char *chardelim = ((pos < this->m_nParams) ? std::strchr(valuechar + len, delim)  // find delimiter
															: (valuechar + std::strlen(valuechar)));

		if (chardelim != nullptr) {
#if defined(_DEBUG)
			const char *charrest = valuechar + len;
			int restlen = (chardelim - charrest);
			if (restlen > 0) {
				char buffer[restlen + 1];
				const char *rest = std::strncpy(buffer, charrest, restlen);

				buffer[restlen] = '\0';

				std::clog << "Warning in parse(): Found \"" << rest << "\" after parameter " << pos << std::endl;
			}
#endif

			p = (chardelim - str) + 1;
		} else {
			std::cerr << "Could not find \"" << delim << "\" after position " << (p + len) << ": " << (valuechar + len) << std::endl;

			p += len;
		}
	}

	return p;
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

int PGparameter::parsevar(const signed int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed int val = std::stoi(std::string(str), &len, 10);

		this->bindany(&val, pos, INT4OID, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const unsigned int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned int val = std::stoi(std::string(str), &len, 10);

		this->bindany(&val, pos, INT4OID, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT4OID, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const signed short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed short int val = std::stoi(std::string(str), &len, 10);

		this->bindany(&val, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const unsigned short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned short int val = std::stoi(std::string(str), &len, 10);

		this->bindany(&val, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT2OID, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const signed long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed long int val = std::stol(str, &len);

		this->bindany(&val, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const unsigned long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned long int val = std::stol(str, &len);

		this->bindany(&val, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const float &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const float val = std::stof(str, &len);

		this->bindany(&val, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, FLOATOID, sizeof(float), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const double &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const double val = std::stod(str, &len);

		this->bindany(&val, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, DOUBLEOID, sizeof(double), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const std::string &value, const int pos, const char *str)
{
	return this->parsevar(value.c_str(), pos, str);
}

int PGparameter::parsevar(const char *value, const int pos, const char *str)
{
	const char *val = ((str != nullptr) ? str : value);

	if (val == nullptr) {
		std::cerr << "PGparameter::parse(): Parsing null string!" << std::endl;

		return 0;
	} else {
		std::size_t len = std::strlen(val);

		this->bindany(val, pos, VARCHAROID, len, FORMAT_TEXT);

		return len;
	}
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
		const char *valuechar = this->m_values[i];
		const parameter *value = reinterpret_cast<const parameter *>(valuechar);
		parameter value2;
		parameter value4;
		parameter value8;

		// Invert bytes...
		value2.m_short = be16toh(value->m_short);
		value4.m_int = be32toh(value->m_int);
		value8.m_long = be64toh(value->m_long);

		if (i != 0) {
			ret += ", ";
		}

		switch (type) {
		case INT4OID:	ret += std::to_string(value4.m_int); break;
		case INT2OID:	ret += std::to_string(value2.m_short) + "h"; break;
		case INT8OID:	ret += std::to_string(value8.m_long) + "l"; break;
		case FLOATOID:	ret += std::to_string(value4.m_float) + "f"; break;
		case DOUBLEOID:	ret += std::to_string(value8.m_double) + "Lf"; break;
		case VARCHAROID:ret = ret + "\"" + valuechar + "\""; break;
		default:		ret = ret + "#" + std::to_string(type) + "#"; break;
		}
	}
	ret += ")";

	return ret;
}


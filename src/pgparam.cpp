
#include <iostream>

#include <cstring>

#include <ctime>

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
		const PARAMETER *value = reinterpret_cast<const PARAMETER *>(valuechar);
		int len = 0;

		switch (type) {
		case INT4OID:		len = this->parsevar(value->m_int, pos, valuechar); break;
		case INT2OID:		len = this->parsevar(value->m_short, pos, valuechar); break;
		case INT8OID:		len = this->parsevar(value->m_long, pos, valuechar); break;
		case FLOATOID:		len = this->parsevar(value->m_float, pos, valuechar); break;
		case DOUBLEOID:		len = this->parsevar(value->m_double, pos, valuechar); break;
		case VARCHAROID:	len = this->parsevar(valuechar, pos, valuechar); break;
		case CHAROID:		len = this->parsevar(value->m_char, pos, valuechar); break;
		case BPCHAROID:		len = this->parsevar(value->m_wchar, pos, valuechar); break;
		case BOOLOID:		len = this->parsevar(value->m_bool, pos, valuechar); break;
		case NUMERICOID:	len = this->parsevar(value->m_numeric, pos, valuechar); break;
		case BYTEAOID:		len = this->parsevar(value->m_bytea, pos, valuechar); break;
		case TIMESTAMPOID:	len = this->parsevar(value->m_timestamp, pos, valuechar); break;
		case TIMEOID:		len = this->parsevar(value->m_time, pos, valuechar); break;
		case TEXTOID:		len = this->parsevar(valuechar, pos, valuechar); break;
		default:			std::cerr << "#" << std::to_string(type) << "#" << std::endl; break;
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

void PGparameter::bindvar(const __int128_t &value, const int pos)
{
	// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

	this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
}

void PGparameter::bindvar(const __uint128_t &value, const int pos)
{
	// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

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

void PGparameter::bindvar(const long double &value, const int pos)
{
	this->bindany(&value, pos, NUMERICOID, sizeof(long double), FORMAT_BINARY);
}

void PGparameter::bindvar(const bool &value, const int pos)
{
	this->bindany(&value, pos, BOOLOID, sizeof(bool), FORMAT_BINARY);
}

void PGparameter::bindvar(const signed char &value, const int pos)
{
	this->bindany(&value, pos, CHAROID, sizeof(signed char), FORMAT_BINARY);
}

void PGparameter::bindvar(const unsigned char &value, const int pos)
{
	this->bindany(&value, pos, CHAROID, sizeof(unsigned char), FORMAT_BINARY);
}

void PGparameter::bindvar(const char16_t &value, const int pos)
{
	this->bindany(&value, pos, BPCHAROID, sizeof(char16_t), FORMAT_BINARY);
}

void PGparameter::bindvar(const char32_t &value, const int pos)
{
	this->bindany(&value, pos, BPCHAROID, sizeof(char32_t), FORMAT_BINARY);
}

void PGparameter::bindvar(const void *value, const int pos)
{
	// The size is not that.

	this->bindany(&value, pos, BYTEAOID, sizeof(void *), FORMAT_BINARY);
}

void PGparameter::bindvar(const std::time_t *value, const int pos)
{
	bool istimestamp = true;  // amount of time greater than a day would work...

	this->bindany(value, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
}

void PGparameter::bindvar(const std::tm *value, const int pos)
{
	const std::time_t val = std::mktime(const_cast<std::tm *>(value));
	bool istimestamp = true;  // amount of time greater than a day would work...

	this->bindany(&val, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
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

int PGparameter::parsevar(const __int128_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed long long int val = std::stoll(str, &len);

		// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

		this->bindany(&val, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, INT8OID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const __uint128_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned long long int val = std::stoll(str, &len);

		// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

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

int PGparameter::parsevar(const long double &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const long double val = std::stold(str, &len);

		// This is just a start! NUMERIC may need some more...

		this->bindany(&val, pos, NUMERICOID, sizeof(long double), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, NUMERICOID, sizeof(long double), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const bool &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = 1;

		const bool val = ((*str == 't') || (*str == 'T'));

		this->bindany(&val, pos, BOOLOID, sizeof(bool), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, BOOLOID, sizeof(bool), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const signed char &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(signed char);

		const signed char val = *str;

		this->bindany(&val, pos, CHAROID, len, FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, CHAROID, len, FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const unsigned char &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(unsigned char);

		const unsigned char val = *str;

		this->bindany(&val, pos, CHAROID, len, FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, CHAROID, len, FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const char16_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(char16_t);

		const unsigned char val = *str;

		this->bindany(&val, pos, BPCHAROID, len, FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, BPCHAROID, len, FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const char32_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(char32_t);

		const unsigned char val = *str;

		this->bindany(&val, pos, BPCHAROID, len, FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, BPCHAROID, len, FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const void *value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const void *val = str;  // Probably not that!

		// The size is not that.

		this->bindany(&val, pos, BYTEAOID, sizeof(void *), FORMAT_BINARY);
	} else {
		this->bindany(&value, pos, BYTEAOID, sizeof(void *), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const std::time_t *value, const int pos, const char *str)
{
	std::size_t len = 0;
	bool istimestamp = true;  // amount of time greater than a day would work...

	if (str != nullptr) {
		const std::time_t val = std::stol(str, &len);

		this->bindany(&val, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
	} else {
		this->bindany(value, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
	}

	return len;
}

int PGparameter::parsevar(const std::tm *value, const int pos, const char *str)
{
	std::size_t len = 0;
	bool istimestamp = true;  // amount of time greater than a day would work...

	if (str != nullptr) {
		[[maybe_unused]] const std::tm *val2 = reinterpret_cast<const std::tm *>(str);  // Probably not that!

		const std::time_t val = std::mktime(const_cast<std::tm *>(val2));

		this->bindany(&val, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
	} else {
		const std::time_t val = std::mktime(const_cast<std::tm *>(value));

		this->bindany(&val, pos, (istimestamp ? TIMESTAMPOID : TIMEOID), sizeof(std::time_t), FORMAT_BINARY);
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
	bool ismemo = false;  // unclear!

	if (val == nullptr) {
		std::cerr << "PGparameter::parse(): Parsing null string!" << std::endl;

		return 0;
	} else {
		std::size_t len = std::strlen(val);

		if (ismemo) {
			this->bindany(val, pos, TEXTOID, len, FORMAT_BINARY);
		} else {
			this->bindany(val, pos, VARCHAROID, len, FORMAT_TEXT);
		}

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
		const PARAMETER *value = reinterpret_cast<const PARAMETER *>(valuechar);
		PARAMETER value2;
		PARAMETER value4;
		PARAMETER value8;
		PARAMETER value16;

		// Invert bytes...
		value2.m_short = be16toh(value->m_short);
		value4.m_int = be32toh(value->m_int);
		value8.m_long = be64toh(value->m_long);
		value16.m_longlong = __swap128(value->m_longlong);

		if (i != 0) {
			ret += ", ";
		}

		switch (type) {
		case INT4OID:		ret += std::to_string(value4.m_int); break;
		case INT2OID:		ret += std::to_string(value2.m_short) + "h"; break;
		case INT8OID:		ret += std::to_string(value8.m_long) + "l"; break;
		case FLOATOID:		ret += std::to_string(value4.m_float) + "f"; break;
		case DOUBLEOID:		ret += std::to_string(value8.m_double) + "Lf"; break;
		case VARCHAROID:	ret += std::string("\"").append(valuechar).append("\""); break;
		case CHAROID:		ret += std::string("'").append(1, value->m_char).append("'"); break;
		case BPCHAROID:		ret += std::string("'").append(1, value->m_wchar).append("'"); break;
		case BOOLOID:		ret += ((value->m_bool == true) ? 'T' : (value->m_bool == false) ? 'F' : '?'); break;
		case NUMERICOID:	ret += std::to_string(value16.m_numeric) + "LLf"; break;
		case BYTEAOID:		ret += static_cast<const char *>(value8.m_bytea); break;
		case TIMESTAMPOID:	ret += trim(std::asctime(std::localtime(&value8.m_timestamp))); break;
		case TIMEOID:		ret += trim(std::asctime(std::localtime(&value8.m_time))); break;
		case TEXTOID:		ret = ret + "\"" + valuechar + "\""; break;
		default:			ret = ret + "#" + std::to_string(type) + "#"; break;
		}
	}
	ret += ")";

	return ret;
}


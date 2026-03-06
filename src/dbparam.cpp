
#include <iostream>

#include <cstring>

#include <endian.h>

#include "dbparam.hpp"

DBparameter::~DBparameter(void)
{
	this->resize(0);  // cleanup
}

void DBparameter::resize(const int nParams)
{
	if (this->m_nParams > 0) {  // remove the old parameters
		if (this->m_types != nullptr) {
			delete[] this->m_types;
		}

		if (this->m_values != nullptr) {
			delete[] this->m_values;
		}

		if (this->m_lengths != nullptr) {
			delete[] this->m_lengths;
		}

		if (this->m_formats != nullptr) {
			delete[] this->m_formats;
		}

		if (this->m_valbuffer != nullptr) {
			delete[] this->m_valbuffer;
		}

#if defined(_DEBUG)
		std::clog << "Deleted old parameter arrays[" << this->m_nParams << "]: " << this << std::endl;
#endif
	}

	this->m_nParams = nParams;

	if (this->m_nParams > 0) {
		this->m_types = new DBparameterType[nParams];
		this->m_values = new const char *[nParams];
		this->m_lengths = new int[nParams];
		this->m_formats = new DBparameterFormat[nParams];
		this->m_valbuffer = new int64_t[nParams];

#if defined(_DEBUG)
		std::clog << "Resized parameter to " << this->m_nParams << " parameters: " << this << std::endl;
#endif
	}
}

void *DBparameter::convertbigendian(const void *value, const int length, const int pos)
{
	if ((pos < 1) || (pos > this->m_nParams)) {
		std::cerr << "Error in convertbigendian(): Illegal index " << pos << "! Should be between 1 and " << this->m_nParams << "..." << std::endl;

		return nullptr;
	}

	void *ret = (this->m_valbuffer + (pos - 1));  // use the reserved space in m_valbuffer for the binary data (not strings)

	switch (length) {
	case 1:	*static_cast<char *>(ret) = *static_cast<const char *>(value);
			break;  // unmodified
	case 2:	*static_cast<int16_t *>(ret) = htobe16(*static_cast<const int16_t *>(value));
			break;
	case 4:	*static_cast<int32_t *>(ret) = htobe32(*static_cast<const int32_t *>(value));
			break;
	case 8:	*static_cast<int64_t *>(ret) = htobe64(*static_cast<const int64_t *>(value));
			break;
	default: ret = nullptr;
			std::cerr << "convertbigendian(): Illegal size (" << length << "!" << std::endl;
			break;
	}

	return ret;
}

void DBparameter::bind(void)
{
	int nParams = - this->m_nParams;

	this->m_nParams = 0;

	this->resize(nParams);
}

int DBparameter::parse(const char *str, const char delim)
{
	int p = 0;

	if (str == nullptr) {
		std::cerr << "DBparameter::parse(): Parsing null string!" << std::endl;

		return p;
	}

	for (int i = 0, pos = i + 1; i < this->m_nParams; ++i, ++pos) {
		DBparameterType type = this->m_types[i];
		const char *valuechar = (str + p);
		const parameter *value = reinterpret_cast<const parameter *>(valuechar);
		int len;

		switch (type) {
		case 1:	len = this->parsevar(value->m_int, pos, valuechar); break;
		case 2:	len = this->parsevar(value->m_short, pos, valuechar); break;
		case 3:	len = this->parsevar(value->m_long, pos, valuechar); break;
		case 4:	len = this->parsevar(value->m_float, pos, valuechar); break;
		case 5:	len = this->parsevar(value->m_double, pos, valuechar); break;
		case 6:	len = this->parsevar(valuechar, pos, valuechar); break;
		default:len = 0; std::cerr << "#" << std::to_string(type) << "#" << std::endl; break;
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

void DBparameter::bindany(const void *value, const int pos, const DBparameterType type, const int length, const DBparameterFormat format)
{
	if ((pos < 1) || (pos > this->m_nParams)) {
		std::cerr << "Error in bindany(): Illegal index " << pos << "! Should be between 1 and " << this->m_nParams << "..." << std::endl;

		return;
	}

	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	this->m_types[i] = type;
	this->m_values[i] = static_cast<const char *>(value);
	this->m_lengths[i] = length;
	this->m_formats[i] = format;
}

void DBparameter::bindbase(const void *value, const int pos, const DBparameterType type, const int length, const DBparameterFormat format)
{
#if defined(_DEBUG)
	std::clog << "Warning! Called bindany() in base parameter class: " << this << std::endl;
#endif

	this->bindany(value, pos, type, length, format);
}

void DBparameter::bindvar(const signed int &value, const int pos)
{
	this->bindbase(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned int &value, const int pos)
{
	this->bindbase(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed short int &value, const int pos)
{
	this->bindbase(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned short int &value, const int pos)
{
	this->bindbase(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed long int &value, const int pos)
{
	this->bindbase(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned long int &value, const int pos)
{
	this->bindbase(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const float &value, const int pos)
{
	this->bindbase(&value, pos, 4, sizeof(float), FORMAT_BINARY);
}

void DBparameter::bindvar(const double &value, const int pos)
{
	this->bindbase(&value, pos, 5, sizeof(double), FORMAT_BINARY);
}

void DBparameter::bindvar(const std::string &value, const int pos)
{
	this->bindvar(value.c_str(), pos);
}

void DBparameter::bindvar(const char *value, const int pos)
{
	this->bindbase(value, pos, 6, std::strlen(value), FORMAT_TEXT);
}

int DBparameter::parsevar(const signed int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, 1, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 1, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, 1, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 1, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const signed short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed short int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, 2, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned short int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, 2, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const signed long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed long int val = std::stol(str, &len);

		this->bindbase(&val, pos, 3, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned long int val = std::stol(str, &len);

		this->bindbase(&val, pos, 3, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const float &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const float val = std::stof(str, &len);

		this->bindbase(&val, pos, 4, sizeof(float), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 4, sizeof(float), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const double &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const double val = std::stod(str, &len);

		this->bindbase(&val, pos, 5, sizeof(double), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, 5, sizeof(double), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const std::string &value, const int pos, const char *str)
{
	return this->parsevar(value.c_str(), pos, str);
}

int DBparameter::parsevar(const char *value, const int pos, const char *str)
{
	const char *val = ((str != nullptr) ? str : value);

	if (val == nullptr) {
		std::cerr << "DBparameter::parse(): Parsing null string!" << std::endl;

		return 0;
	} else {
		std::size_t len = std::strlen(val);

		this->bindbase(val, pos, 6, len, FORMAT_TEXT);

		return len;
	}

}

int DBparameter::count(void) const
{
	return this->m_nParams;
}

const DBparameterType *DBparameter::types(void) const
{
	return this->m_types;
}

const char **DBparameter::values(void) const
{
	return this->m_values;
}

const int *DBparameter::lengths(void) const
{
	return this->m_lengths;
}

const int *DBparameter::formats(void) const
{
	// This is a bit hacky, but it allows us to return the formats as an array of ints, which is what the DBparameterFormat enum is.
	return reinterpret_cast<int *>(this->m_formats);
}

std::string DBparameter::to_string(void) const
{
	std::string ret = std::string("[") + std::to_string(this->m_nParams) + "](";

	for (int i = 0; i < this->m_nParams; ++i) {
		DBparameterType type = this->m_types[i];
		const char *valuechar = this->m_values[i];
		const parameter *value = reinterpret_cast<const parameter *>(valuechar);

		if (i != 0) {
			ret += ", ";
		}

		switch (type) {
		case 1:	ret += std::to_string(value->m_int); break;
		case 2:	ret += std::to_string(value->m_short) + "h"; break;
		case 3:	ret += std::to_string(value->m_long) + "l"; break;
		case 4:	ret += std::to_string(value->m_float) + "f"; break;
		case 5:	ret += std::to_string(value->m_double) + "Lf"; break;
		case 6:	ret = ret + "\"" + valuechar + "\""; break;
		default:ret = ret + "#" + std::to_string(type) + "#"; break;
		}
	}
	ret += ")";

	return ret;
}


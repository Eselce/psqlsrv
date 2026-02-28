
#include <iostream>

#include <cstring>

#include <endian.h>

#include "dbparam.hpp"

DBparameter::DBparameter(const int nParams)
:	m_nParams(0),
	m_types(nullptr),
	m_values(nullptr),
	m_lengths(nullptr),
	m_formats(nullptr),
	m_valbuffer(nullptr)
{
	this->resize(nParams);
}

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

void DBparameter::bind(void) {
	int nParams = - this->m_nParams;

	this->m_nParams = 0;

	this->resize(nParams);
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

void DBparameter::bindvar(const signed int &value, const int pos)
{
	this->bindany(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned int &value, const int pos)
{
	this->bindany(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed short int &value, const int pos)
{
	this->bindany(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned short int &value, const int pos)
{
	this->bindany(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed long int &value, const int pos)
{
	this->bindany(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned long int &value, const int pos)
{
	this->bindany(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const float &value, const int pos)
{
	this->bindany(&value, pos, 4, sizeof(float), FORMAT_BINARY);
}

void DBparameter::bindvar(const double &value, const int pos)
{
	this->bindany(&value, pos, 5, sizeof(double), FORMAT_BINARY);
}

void DBparameter::bindvar(const std::string &value, const int pos)
{
	this->bindvar(value.c_str(), pos);
}

void DBparameter::bindvar(const char *value, const int pos)
{
	this->bindany(value, pos, 6, std::strlen(value), FORMAT_TEXT);
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


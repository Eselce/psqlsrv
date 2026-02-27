
#include <iostream>

#include <cstring>

#include <endian.h>

#include "dbparam.hpp"

DBparameter::DBparameter(const int nParams)
{
	this->m_nParams = nParams;

	this->m_types = new DBparameterType[nParams];
	this->m_values = new const char *[nParams];
	this->m_lengths = new int[nParams];
	this->m_formats = new DBparameterFormat[nParams];
}

DBparameter::~DBparameter(void)
{
	delete[] this->m_types;
	delete[] this->m_values;
	delete[] this->m_lengths;
	delete[] this->m_formats;

#if defined(_DEBUG)
	std::clog << "Deleted parameter arrays[" << this->m_nParams << "]: " << this << std::endl;
#endif
}

void DBparameter::resize(const int nParams)
{
	this->m_nParams = nParams;

	delete[] this->m_types;
	delete[] this->m_values;
	delete[] this->m_lengths;
	delete[] this->m_formats;

#if defined(_DEBUG)
	std::clog << "Deleted old parameter arrays: " << this << std::endl;
#endif

	this->m_types = new DBparameterType[nParams];
	this->m_values = new const char *[nParams];
	this->m_lengths = new int[nParams];
	this->m_formats = new DBparameterFormat[nParams];

#if defined(_DEBUG)
	std::clog << "Resized parameter to " << nParams << " parameters: " << this << std::endl;
#endif
}

void *DBparameter::convertbigendian(const void *value, const int length)
{
	void *ret = const_cast<void *>(value);  // TODO: We may have to allocate memory here instead!

	switch (length) {
	case 1:	break;  // nothing to do
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

void DBparameter::bindany(const void *value, const int pos, const DBparameterType type, const int length, const DBparameterFormat format)
{
	const int i = pos - 1;  // pos is 1-based, but arrays are 0-based

	this->m_types[i] = type;
	this->m_values[i] = static_cast<const char *>(value);
	this->m_lengths[i] = length;
	this->m_formats[i] = format;
}

void DBparameter::bind(const int &value, const int pos)
{
	this->bindany(&value, pos, 1, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bind(const short int &value, const int pos)
{
	this->bindany(&value, pos, 2, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bind(const long int &value, const int pos)
{
	this->bindany(&value, pos, 3, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bind(const float &value, const int pos)
{
	this->bindany(&value, pos, 4, sizeof(float), FORMAT_BINARY);
}

void DBparameter::bind(const double &value, const int pos)
{
	this->bindany(&value, pos, 5, sizeof(double), FORMAT_BINARY);
}

void DBparameter::bind(const std::string &value, const int pos)
{
	this->bind(value.c_str(), pos);
}

void DBparameter::bind(const char *value, const int pos)
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
	return const_cast<const int *>(reinterpret_cast<int *>(this->m_formats));
}


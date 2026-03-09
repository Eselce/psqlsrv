
#include <iostream>

#include <cstring>

#include <ctime>

#include <algorithm>

#include <ranges>

#include <endian.h>

#include "dbparam.hpp"

static const DBparameterType INT40ID		= 1;
static const DBparameterType INT20ID		= 2;
static const DBparameterType INT80ID		= 3;
static const DBparameterType FLOAT0ID		= 4;
static const DBparameterType DOUBLE0ID		= 5;
static const DBparameterType VARCHAR0ID		= 6;
static const DBparameterType CHAR0ID		= 7;
static const DBparameterType BPCHAR0ID		= 8;
static const DBparameterType BOOL0ID		= 9;
static const DBparameterType NUMERIC0ID		= 10;
static const DBparameterType BYTEA0ID		= 11;
static const DBparameterType TIMESTAMP0ID	= 12;
static const DBparameterType TIME0ID		= 13;
static const DBparameterType TEXT0ID		= 14;

typedef union
{
	__int128_t m128;
	int64_t a64[2];
}	swapper128;

__int128_t __swap128(const __int128_t &src128)
{
	swapper128 src, dst;

	src.m128 = src128;
	dst.a64[0] = be64toh(src.a64[1]);
	dst.a64[1] = be64toh(src.a64[0]);

	return dst.m128;
}

std::string trim(std::string &str)
{
	auto view = str
				| std::views::drop_while(isspace)
				| std::views::reverse
				| std::views::drop_while(isspace)
				| std::views::reverse;

	auto [in, out] = std::ranges::copy(view, str.begin());
	str.erase(out, str.end());

	return str;
}

std::string trim(const char *str)
{
	std::string temp(str);

	return trim(temp);
}

/***
void trim0(std::string& s) {
  auto view = s
      | std::views::drop_while(isspace)
      | std::views::reverse
      | std::views::drop_while(isspace)
      | std::views::reverse;
  auto [in, out] = std::ranges::copy(view, s.begin());
  s.erase(out, s.end());
}
***/

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
		this->m_valbuffer = new PARAMETER[nParams];

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
	case 16:*static_cast<__int128_t *>(ret) = __swap128(*static_cast<const __int128_t *>(value));
			break;
	default:ret = nullptr;
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
		const PARAMETER *value = reinterpret_cast<const PARAMETER *>(valuechar);
		int len = 0;

		switch (type) {
		case INT40ID:		len = this->parsevar(value->m_int, pos, valuechar); break;
		case INT20ID:		len = this->parsevar(value->m_short, pos, valuechar); break;
		case INT80ID:		len = this->parsevar(value->m_long, pos, valuechar); break;
		case FLOAT0ID:		len = this->parsevar(value->m_float, pos, valuechar); break;
		case DOUBLE0ID:		len = this->parsevar(value->m_double, pos, valuechar); break;
		case VARCHAR0ID:	len = this->parsevar(valuechar, pos, valuechar); break;
		case CHAR0ID:		len = this->parsevar(value->m_char, pos, valuechar); break;
		case BPCHAR0ID:		len = this->parsevar(value->m_wchar, pos, valuechar); break;
		case BOOL0ID:		len = this->parsevar(value->m_bool, pos, valuechar); break;
		case NUMERIC0ID:	len = this->parsevar(value->m_numeric, pos, valuechar); break;
		case BYTEA0ID:		len = this->parsevar(value->m_bytea, pos, valuechar); break;
		case TIMESTAMP0ID:	len = this->parsevar(value->m_timestamp, pos, valuechar); break;
		case TIME0ID:		len = this->parsevar(value->m_time, pos, valuechar); break;
		case TEXT0ID:		len = this->parsevar(valuechar, pos, valuechar); break;
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
	this->bindbase(&value, pos, INT40ID, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned int &value, const int pos)
{
	this->bindbase(&value, pos, INT40ID, sizeof(int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed short int &value, const int pos)
{
	this->bindbase(&value, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned short int &value, const int pos)
{
	this->bindbase(&value, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed long int &value, const int pos)
{
	this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned long int &value, const int pos)
{
	this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const __int128_t &value, const int pos)
{
	// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

	this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const __uint128_t &value, const int pos)
{
	// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

	this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
}

void DBparameter::bindvar(const float &value, const int pos)
{
	this->bindbase(&value, pos, FLOAT0ID, sizeof(float), FORMAT_BINARY);
}

void DBparameter::bindvar(const double &value, const int pos)
{
	this->bindbase(&value, pos, DOUBLE0ID, sizeof(double), FORMAT_BINARY);
}

void DBparameter::bindvar(const long double &value, const int pos)
{
	this->bindbase(&value, pos, NUMERIC0ID, sizeof(long double), FORMAT_BINARY);
}

void DBparameter::bindvar(const bool &value, const int pos)
{
	this->bindbase(&value, pos, BOOL0ID, sizeof(bool), FORMAT_BINARY);
}

void DBparameter::bindvar(const signed char &value, const int pos)
{
	this->bindbase(&value, pos, CHAR0ID, sizeof(signed char), FORMAT_BINARY);
}

void DBparameter::bindvar(const unsigned char &value, const int pos)
{
	this->bindbase(&value, pos, CHAR0ID, sizeof(unsigned char), FORMAT_BINARY);
}

void DBparameter::bindvar(const char16_t &value, const int pos)
{
	this->bindbase(&value, pos, BPCHAR0ID, sizeof(char16_t), FORMAT_BINARY);
}

void DBparameter::bindvar(const char32_t &value, const int pos)
{
	this->bindbase(&value, pos, BPCHAR0ID, sizeof(char32_t), FORMAT_BINARY);
}

void DBparameter::bindvar(const void *value, const int pos)
{
	// The size is not that.

	this->bindbase(&value, pos, BYTEA0ID, sizeof(void *), FORMAT_BINARY);
}

void DBparameter::bindvar(const std::time_t *value, const int pos)
{
	bool istimestamp = true;  // amount of time greater than a day would work...

	this->bindbase(value, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
}

void DBparameter::bindvar(const std::tm *value, const int pos)
{
	const std::time_t val = std::mktime(const_cast<std::tm *>(value));
	bool istimestamp = true;  // amount of time greater than a day would work...

	this->bindbase(&val, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
}

void DBparameter::bindvar(const std::string &value, const int pos)
{
	this->bindvar(value.c_str(), pos);
}

void DBparameter::bindvar(const char *value, const int pos)
{
	this->bindbase(value, pos, VARCHAR0ID, std::strlen(value), FORMAT_TEXT);
}

int DBparameter::parsevar(const signed int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, INT40ID, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT40ID, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, INT40ID, sizeof(int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT40ID, sizeof(int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const signed short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed short int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned short int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned short int val = std::stoi(std::string(str), &len, 10);

		this->bindbase(&val, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT20ID, sizeof(short int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const signed long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed long int val = std::stol(str, &len);

		this->bindbase(&val, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned long int &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned long int val = std::stol(str, &len);

		this->bindbase(&val, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const __int128_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const signed long long int val = std::stoll(str, &len);

		// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

		this->bindbase(&val, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const __uint128_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const unsigned long long int val = std::stoll(str, &len);

		// Okay, now here are several alternative types: INT8OID, TIMESTAMPOID

		this->bindbase(&val, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, INT80ID, sizeof(long int), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const float &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const float val = std::stof(str, &len);

		this->bindbase(&val, pos, FLOAT0ID, sizeof(float), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, FLOAT0ID, sizeof(float), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const double &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const double val = std::stod(str, &len);

		this->bindbase(&val, pos, DOUBLE0ID, sizeof(double), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, DOUBLE0ID, sizeof(double), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const long double &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const long double val = std::stold(str, &len);

		// This is just a start! NUMERIC may need some more...

		this->bindbase(&val, pos, NUMERIC0ID, sizeof(long double), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, NUMERIC0ID, sizeof(long double), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const bool &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = 1;

		const bool val = ((*str == 't') || (*str == 'T'));

		this->bindbase(&val, pos, BOOL0ID, sizeof(bool), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, BOOL0ID, sizeof(bool), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const signed char &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(signed char);

		const signed char val = *str;

		this->bindbase(&val, pos, CHAR0ID, len, FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, CHAR0ID, len, FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const unsigned char &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(unsigned char);

		const unsigned char val = *str;

		this->bindbase(&val, pos, CHAR0ID, len, FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, CHAR0ID, len, FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const char16_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(char16_t);

		const unsigned char val = *str;

		this->bindbase(&val, pos, BPCHAR0ID, len, FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, BPCHAR0ID, len, FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const char32_t &value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		len = sizeof(char32_t);

		const unsigned char val = *str;

		this->bindbase(&val, pos, BPCHAR0ID, len, FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, BPCHAR0ID, len, FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const void *value, const int pos, const char *str)
{
	std::size_t len = 0;

	if (str != nullptr) {
		const void *val = str;  // Probably not that!

		// The size is not that.

		this->bindbase(&val, pos, BYTEA0ID, sizeof(void *), FORMAT_BINARY);
	} else {
		this->bindbase(&value, pos, BYTEA0ID, sizeof(void *), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const std::time_t *value, const int pos, const char *str)
{
	std::size_t len = 0;
	bool istimestamp = true;  // amount of time greater than a day would work...

	if (str != nullptr) {
		const std::time_t val = std::stol(str, &len);

		this->bindbase(&val, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
	} else {
		this->bindbase(value, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
	}

	return len;
}

int DBparameter::parsevar(const std::tm *value, const int pos, const char *str)
{
	std::size_t len = 0;
	bool istimestamp = true;  // amount of time greater than a day would work...

	if (str != nullptr) {
		[[maybe_unused]] const std::tm *val2 = reinterpret_cast<const std::tm *>(str);  // Probably not that!

		const std::time_t val = std::mktime(const_cast<std::tm *>(val2));

		this->bindbase(&val, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
	} else {
		const std::time_t val = std::mktime(const_cast<std::tm *>(value));

		this->bindbase(&val, pos, (istimestamp ? TIMESTAMP0ID : TIME0ID), sizeof(std::time_t), FORMAT_BINARY);
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

		this->bindbase(val, pos, VARCHAR0ID, len, FORMAT_TEXT);

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
		const PARAMETER *value = reinterpret_cast<const PARAMETER *>(valuechar);

		if (i != 0) {
			ret += ", ";
		}

		switch (type) {
		case INT40ID:		ret += std::to_string(value->m_int); break;
		case INT20ID:		ret += std::to_string(value->m_short) + "h"; break;
		case INT80ID:		ret += std::to_string(value->m_long) + "l"; break;
		case FLOAT0ID:		ret += std::to_string(value->m_float) + "f"; break;
		case DOUBLE0ID:		ret += std::to_string(value->m_double) + "Lf"; break;
		case VARCHAR0ID:	ret += std::string("\"").append(valuechar).append("\""); break;
		case CHAR0ID:		ret += std::string("'").append(1, value->m_char).append("'"); break;
		case BPCHAR0ID:		ret += std::string("'").append(1, value->m_wchar).append("'"); break;
		case BOOL0ID:		ret += ((value->m_bool == true) ? 'T' : (value->m_bool == false) ? 'F' : '?'); break;
		case NUMERIC0ID:	ret += std::to_string(value->m_numeric) + "LLf"; break;
		case BYTEA0ID:		ret += static_cast<const char *>(value->m_bytea); break;
		case TIMESTAMP0ID:	ret += trim(std::asctime(std::localtime(&value->m_timestamp))); break;
		case TIME0ID:		ret += trim(std::asctime(std::localtime(&value->m_time))); break;
		case TEXT0ID:		ret = ret + "\"" + valuechar + "\""; break;
		default:			ret = ret + "#" + std::to_string(type) + "#"; break;
		}
	}
	ret += ")";

	return ret;
}


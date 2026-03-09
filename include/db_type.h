
#pragma once

#include <ctime>

// General C++ type for type parameter in DBparameter::bindany()...
typedef unsigned int DBparameterType;

// Possible values for format parameter in DBparameter::bindany()...
typedef enum
{
	FORMAT_TEXT	= 0,			// 0: FORMAT_TEXT   - Text format
	FORMAT_BINARY				// 1: FORMAT_BINARY - Binary format
}	DBparameterFormat;

[[maybe_unused]] static const char *DBparameterFormatName[] =
{
	"FORMAT_TEXT",				// 0: FORMAT_TEXT   - Text format
	"FORMAT_BINARY"				// 1: FORMAT_BINARY - Binary format
};

typedef union
{
	signed int s_int;
	unsigned int u_int;
	int m_int;

	signed short int s_short;
	unsigned short int u_short;
	short int m_short;

	signed long int s_long;
	unsigned long int u_long;
	long int m_long;

	__int128_t s_longlong;
	__uint128_t u_longlong;
	__int128_t m_longlong;

	float m_float;

	double m_double;

	long double m_numeric;

	char8_t m_char;
	char16_t m_wchar;
	char32_t m_wwchar;

	bool m_bool;

	//memo m_text;
	//varchar m_str;
	void *m_bytea;

	std::time_t m_timestamp;

	std::tm m_tm;

	std::time_t m_time;

	//date m_date;
}	PARAMETER;

class DBconnection;				// forward declaration to avoid circular dependency

extern const char *cmdErrorMsg;	// = "Command failed";

extern const char *cmdLogMsg;	// = "Execute command (status)";

extern __int128_t __swap128(const __int128_t &src128);

extern std::string trim(std::string &str);

extern std::string trim(const char *str);


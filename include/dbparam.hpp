
#pragma once

#include <string>

// General C++ type for type parameter in DBparameter::setany()...
typedef unsigned int DBparameterType;

// Possible values for format parameter in DBparameter::setany()...
typedef enum
{
	FORMAT_TEXT	= 0,	// 0: FORMAT_TEXT   - Text format
	FORMAT_BINARY		// 1: FORMAT_BINARY - Binary format
} DBparameterFormat;

class DBparameter
{
public:
	DBparameter(const int nParams = 0);

	virtual ~DBparameter(void);

public:
	virtual void set(const int &value, const int pos);

	virtual void set(const short int &value, const int pos);

	virtual void set(const long int &value, const int pos);

	virtual void set(const float &value, const int pos);

	virtual void set(const double &value, const int pos);

	virtual void set(const std::string &value, const int pos);

	virtual void set(const char *value, const int pos);

	virtual int count(void) const;

	virtual const DBparameterType *types(void) const;

	virtual const char * const *values(void) const;

	virtual const int *lengths(void) const;

	virtual const int *formats(void) const;

protected:
	virtual void resize(const int nParams);

	virtual void setany(const void *value, const int pos, const DBparameterType type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY);

protected:
	int m_nParams;

	DBparameterType *m_types;

	const char * *m_values;

	int *m_lengths;

	DBparameterFormat *m_formats;
};


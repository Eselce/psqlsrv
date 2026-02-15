
#pragma once

#include <string>

typedef unsigned int DBtype;

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

	virtual const DBtype *types(void) const;

	virtual const char * const *values(void) const;

	virtual const int *lengths(void) const;

	virtual const int *formats(void) const;

protected:
	virtual void resize(const int nParams);

	virtual void setany(const void *value, const int pos, const DBtype type, const int length = 0, const int format = 0);

protected:
	int m_nParams;

	DBtype *m_types;

	const char * *m_values;

	int *m_lengths;

	int *m_formats;
};


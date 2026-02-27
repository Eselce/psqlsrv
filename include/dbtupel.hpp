
#pragma once

#include <string>

#include "db_type.h"

class DBtupel
{
public:
	DBtupel(const int nFields = 0);

	virtual ~DBtupel(void);

public:
	virtual void bindvar(const int &value, const int pos);

	virtual void bindvar(const short int &value, const int pos);

	virtual void bindvar(const long int &value, const int pos);

	virtual void bindvar(const float &value, const int pos);

	virtual void bindvar(const double &value, const int pos);

	virtual void bindvar(const std::string &value, const int pos);

	virtual void bindvar(const char *value, const int pos);

	virtual int count(void) const;

	virtual const DBparameterType *types(void) const;

	virtual const char **values(void) const;

	virtual const int *lengths(void) const;

	virtual const int *formats(void) const;

protected:
	virtual void resize(const int nFields);

	virtual void *convertlittleendian(const void *value, const int length, const int pos = 1);

	virtual void bindany(const void *value, const int pos, const DBparameterType type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY);

protected:
	int m_nFields;

	DBparameterType *m_types;

	const char **m_values;

	int *m_lengths;

	DBparameterFormat *m_formats;

	int64_t *m_valbuffer;
};


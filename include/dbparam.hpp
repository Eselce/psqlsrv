
#pragma once

#include <iostream>

#include <string>

#include "db_type.h"

class DBparameter
{
public:
	DBparameter(const int nParams = 0);

	virtual ~DBparameter(void);

public:
	virtual void bind(void);

	template <typename T, typename... Args>
	void bind(T value, Args... args) {
		if (this->m_nParams > 0) {
			std::cerr << "Error: Can't call bind() with memory allocated!" << std::endl;
			return;
		}
		int pos = - (--this->m_nParams);
    	this->bind(args...);
		this->bindvar(value, pos);
	};

	virtual void bindvar(const signed int &value, const int pos);
	virtual void bindvar(const unsigned int &value, const int pos);

	virtual void bindvar(const signed short int &value, const int pos);
	virtual void bindvar(const unsigned short int &value, const int pos);

	virtual void bindvar(const signed long int &value, const int pos);
	virtual void bindvar(const unsigned long int &value, const int pos);

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
	virtual void resize(const int nParams);

	virtual void *convertbigendian(const void *value, const int length, const int pos = 1);

	virtual void bindany(const void *value, const int pos, const DBparameterType type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY);

protected:
	int m_nParams;

	DBparameterType *m_types;

	const char **m_values;

	int *m_lengths;

	DBparameterFormat *m_formats;

	int64_t *m_valbuffer;
};


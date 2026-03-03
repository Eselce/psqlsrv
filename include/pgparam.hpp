
#pragma once

#include "pg_type.h"

#include "dbparam.hpp"

class PGparameter : public DBparameter
{
public:
	template <typename... Args>
	PGparameter(Args... args)
	:	DBparameter(args...)
	{
		this->bind(args...);
	}

	virtual ~PGparameter(void) override;

public:
	virtual void bindvar(const signed int &value, const int pos) override;
	virtual void bindvar(const unsigned int &value, const int pos) override;

	virtual void bindvar(const signed short int &value, const int pos) override;
	virtual void bindvar(const unsigned short int &value, const int pos) override;

	virtual void bindvar(const signed long int &value, const int pos) override;
	virtual void bindvar(const unsigned long int &value, const int pos) override;

	virtual void bindvar(const float &value, const int pos) override;

	virtual void bindvar(const double &value, const int pos) override;

	virtual void bindvar(const std::string &value, const int pos) override;

	virtual void bindvar(const char *value, const int pos) override;

	virtual const Oid *types(void) const override;

	virtual std::string to_string(void) const override;

protected:
	virtual void bindany(const void *value, const int pos, const Oid type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY) override;

protected:
};


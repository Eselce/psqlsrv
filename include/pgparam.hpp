
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbparam.hpp"

class PGparameter : public DBparameter
{
public:
	PGparameter(const int nParams = 0);

	virtual ~PGparameter(void) override;

public:
	virtual void set(const int &value, const int pos) override;

	virtual void set(const short int &value, const int pos) override;

	virtual void set(const long int &value, const int pos) override;

	virtual void set(const float &value, const int pos) override;

	virtual void set(const double &value, const int pos) override;

	virtual void set(const std::string &value, const int pos) override;

	virtual void set(const char *value, const int pos) override;

	virtual const Oid *types(void) const override;

protected:
	virtual void setany(const void *value, const int pos, const Oid type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY) override;

protected:
};


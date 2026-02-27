
#pragma once

#include "pg_type.h"

#include "dbtupel.hpp"

class PGtupel : public DBtupel
{
public:
	PGtupel(const int nFields = 0);

	virtual ~PGtupel(void) override;

public:
	virtual void bindvar(const int &value, const int pos) override;

	virtual void bindvar(const short int &value, const int pos) override;

	virtual void bindvar(const long int &value, const int pos) override;

	virtual void bindvar(const float &value, const int pos) override;

	virtual void bindvar(const double &value, const int pos) override;

	virtual void bindvar(const std::string &value, const int pos) override;

	virtual void bindvar(const char *value, const int pos) override;

	virtual const Oid *types(void) const override;

protected:
	virtual void bindany(const void *value, const int pos, const Oid type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY) override;

protected:
};


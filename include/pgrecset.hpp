
#pragma once

#include "postgresql/libpq-fe.h"

#include "pgconn.hpp"

#include "pgparam.hpp"

#include "dbrecset.hpp"

class PGrecordset : public DBrecordset
{
public:
	PGrecordset(DBconnection *conn, const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	PGrecordset(PGconnection *conn, const char *command, const int nParams = 0, const Oid *paramTypes = nullptr);

	virtual ~PGrecordset(void) override;

	virtual void createStatement(const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr) override;

protected:
};


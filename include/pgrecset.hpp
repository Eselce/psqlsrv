
#pragma once

#include "pg_type.h"

#include "pgconn.hpp"

#include "pgparam.hpp"

#include "dbrecset.hpp"

class PGrecordset : public DBrecordset
{
public:
	PGrecordset(DBconnection *conn,
				const char *command,
				const int nParams = 0,
				const DBparameterType *paramTypes = nullptr,
				const char *stmtName = nullptr);

	PGrecordset(PGconnection *conn,
				const char *command,
				const int nParams = 0,
				const Oid *paramTypes = nullptr,
				const char *stmtName = nullptr);

	virtual ~PGrecordset(void) override;

	virtual void createStatement(const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr, const char *stmtName = nullptr) override;

	virtual bool first(void) override;

	virtual bool last(void) override;

	virtual bool next(void) override;

	virtual bool prev(void) override;

	virtual const DBparameterType *getFieldTypes(void) const override;

	virtual std::string getFieldName(const int field) const override;

	virtual int getFieldNumber(const std::string fieldName) const override;

protected:
};


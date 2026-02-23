
#pragma once

#include "pg_type.h"

#include "dbstmt.hpp"

class PGstatement : public DBstatement
{
public:
	PGstatement(const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~PGstatement(void) override;

	virtual std::string getautoname(void) const override;

	virtual int getFieldNumber(const std::string fieldName) const override;

	virtual void prepare(DBconnection *conn) override;

	virtual void prepare(PGconnection *conn);

	virtual const DBanswer *exec(DBconnection *conn, const char *errmsg, const DBparameterFormat resultFormat) override;

	virtual const DBanswer *exec(PGconnection *conn, const char *errmsg, const DBparameterFormat resultFormat);

	virtual const DBanswer *exec(DBconnection *conn, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat) override;

	virtual const DBanswer *exec(PGconnection *conn, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat);

protected:
	virtual void calcFieldInfos(DBconnection *conn) override;

	virtual void calcFieldInfos(PGconnection *conn);

protected:
	static const std::string m_autoname_prefix;
	static int m_autoname_index;

protected:
	PGresult *m_pRes;
};


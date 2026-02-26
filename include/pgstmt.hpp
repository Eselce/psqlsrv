
#pragma once

#include "pg_type.h"

#include "dbstmt.hpp"

class PGstatement : public DBstatement
{
public:
	PGstatement(DBconnection *conn, const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~PGstatement(void) override;

	virtual std::string getautoname(void) const override;

	virtual int getFieldNumber(const std::string fieldName) const override;

	virtual void prepare(void) override;

protected:
	virtual void calcFieldInfos(void) override;

protected:
	static const std::string m_autoname_prefix;
	static int m_autoname_index;

protected:
	PGresult *m_pRes;
};


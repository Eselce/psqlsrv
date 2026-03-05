
#pragma once

#include "pg_type.h"

#include "pgparam.hpp"

#include "dbstmt.hpp"

class PGstatement : public DBstatement
{
public:
	PGstatement(DBconnection *conn, const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	template <typename... Args>
	PGstatement(DBconnection *conn, const std::string &command = "", Args... args)
	:	DBstatement(conn, command),
		m_pRes(nullptr),
		m_pParam(new PGparameter(args...))
	{
		this->resize(m_pParam->count());

		this->m_paramTypes = m_pParam->types();
	}

	virtual ~PGstatement(void) override;

	virtual std::string getautoname(void) const override;

	virtual int getFieldNumber(const std::string fieldName) const override;

	virtual void prepare(void) override;

protected:
	virtual void calcFieldInfos(void) override;

	virtual DBparameter *getParam(void) override;

protected:
	static const std::string m_autoname_prefix;
	static int m_autoname_index;

protected:
	PGresult *m_pRes;

	DBparameter *m_pParam;
};


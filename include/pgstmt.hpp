
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbstmt.hpp"

class PGstatement : public DBstatement
{
public:
	PGstatement(const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~PGstatement(void) override;

	virtual const std::string getautoname(void) const override;

protected:
	static const std::string m_autoname_prefix;
	static int m_autoname_index;

protected:
};


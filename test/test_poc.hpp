
#pragma once

#include "pg_type.h"

#include "pgconn.hpp"

class test_poc
{
public:
	test_poc(void);

	virtual ~test_poc(void);

public:
	virtual bool run(void);

protected:
	virtual bool classrun(PGconnection &pg, const char *command, const int arg) const;

	virtual bool autorun(PGconnection &pg, const char *command, const int arg) const;

	virtual bool manualrun(PGconnection &pg, const char *command, const int arg) const;

	virtual void freestmt(PGconnection &pg, const char *stmtname) const;
};


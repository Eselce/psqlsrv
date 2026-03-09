
#pragma once

#include "pg_type.h"

class test_param
{
public:
	test_param(void);

	virtual ~test_param(void);

public:
	virtual bool run(void);

protected:
	virtual bool paramrun(PGconnection &pg, const char *command, const int arg) const;

	virtual bool selectemptyrun(PGconnection &pg, const char *command, const int arg) const;

	virtual bool insertrun(PGconnection &pg, const char *command, const int arg) const;
};


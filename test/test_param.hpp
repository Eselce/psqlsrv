
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
	virtual bool classrun(PGconnection &pg, const char *command, const int arg) const;
};


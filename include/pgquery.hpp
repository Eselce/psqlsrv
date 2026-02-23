
#pragma once

#include "pg_type.h"

#include "dbquery.hpp"

class PGquery : public DBquery
{
public:
	PGquery(void);

	virtual ~PGquery(void) override;

protected:
};



#pragma once

#include "postgresql/libpq-fe.h"

#include "dbrecset.hpp"

class PGrecordset : public DBrecordset
{
public:
	PGrecordset(void);

	virtual ~PGrecordset(void);

protected:
};


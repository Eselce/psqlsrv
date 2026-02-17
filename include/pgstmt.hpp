
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbstmt.hpp"

class PGstatement : public DBstatement
{
public:
	PGstatement(void);

	virtual ~PGstatement(void) override;

protected:
};


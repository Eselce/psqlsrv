
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbanswer.hpp"

class PGanswer : public DBanswer
{
public:
	PGanswer(PGresult *res);

	virtual ~PGanswer(void);

protected:
	PGresult *m_pRes;
};


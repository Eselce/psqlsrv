
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbanswer.hpp"

class PGanswer : public DBanswer
{
public:
	PGanswer(PGresult *res);

	virtual ~PGanswer(void);

public:
	virtual const std::string getanswer(void) const override;

protected:
	PGresult *m_pRes;
};


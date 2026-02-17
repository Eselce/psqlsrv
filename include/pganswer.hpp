
#pragma once

#include "postgresql/libpq-fe.h"

#include "dbanswer.hpp"

class PGanswer : public DBanswer
{
public:
	PGanswer(PGresult *res);

	virtual ~PGanswer(void) override;

public:
	virtual const std::string getanswer(const DBparameterFormat resultFormat = FORMAT_TEXT) const override;

protected:
	PGresult *m_pRes;
};


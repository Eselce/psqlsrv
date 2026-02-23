
#pragma once

#include "pg_type.h"

#include "dbanswer.hpp"

class PGanswer : public DBanswer
{
public:
	PGanswer(PGresult *res);

	virtual ~PGanswer(void) override;

public:
	virtual std::string getanswer(const DBparameterFormat resultFormat = FORMAT_TEXT) const override;

protected:
	virtual std::string gettexttable(void) const override;

	virtual std::string getbinarytable(void) const override;

protected:
	PGresult *m_pRes;
};


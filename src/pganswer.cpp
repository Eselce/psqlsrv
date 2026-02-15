
#include "pganswer.hpp"

PGanswer::PGanswer(PGresult *res)
:	m_pRes(res)
{
}

PGanswer::~PGanswer(void)
{
	PQclear(m_pRes);
}


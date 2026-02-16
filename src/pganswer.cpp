
#include <string>

#include "pganswer.hpp"

PGanswer::PGanswer(PGresult *res)
:	m_pRes(res)
{
}

PGanswer::~PGanswer(void)
{
	PQclear(m_pRes);
}

const std::string PGanswer::getanswer(void) const
{
	ExecStatusType status = PQresultStatus(m_pRes);

	if (status == PGRES_TUPLES_OK) {
		int ntuples = PQntuples(m_pRes);
		int nfields = PQnfields(m_pRes);
		std::string answer = "";

		for (int row = 0; row < ntuples; ++row) {
			for (int col = 0; col < nfields; ++col) {
				char *value = PQgetvalue(m_pRes, row, col);

				answer += (value ? value : "NULL");

				if (col < (nfields - 1)) {
					answer += ",";
				}
			}

			if (row < (ntuples - 1)) {
				answer += "\n";
			}
		}
		return answer;
	} else if (status == PGRES_COMMAND_OK) {
		return "OK";
	} else {
		return "";
	}
}



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

const std::string PGanswer::getanswer(const DBparameterFormat resultFormat) const
{
	ExecStatusType status = PQresultStatus(m_pRes);
	std::string answer = "";

	if (status == PGRES_TUPLES_OK) {
		if (resultFormat == FORMAT_TEXT) {
			const int ntuples = PQntuples(m_pRes);
			const int nfields = PQnfields(m_pRes);

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
		} else if (resultFormat == FORMAT_BINARY) {
			answer = "BINARY DATA";
		} else {
			answer = "UNKNOWN FORMAT";
		}
	} else if (status == PGRES_COMMAND_OK) {
		answer = "OK";
	}

	return answer;
}


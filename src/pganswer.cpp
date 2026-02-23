
#include <string>
#include <iostream>

#include "pg_type.h"

#include "pganswer.hpp"

PGanswer::PGanswer(PGresult *res)
:	m_pRes(res)
{
}

PGanswer::~PGanswer(void)
{
    if (m_pRes != nullptr) {
        PQclear(m_pRes);

#if defined(_DEBUG)
		std::clog << "Cleared result: " << this << std::endl;
		m_pRes = nullptr;
#endif
    }
}

std::string PGanswer::getanswer(const DBparameterFormat resultFormat) const
{
	ExecStatusType status = PQresultStatus(m_pRes);
	std::string answer = "";

#if defined(_DEBUG)
    std::clog << "getanswer() got status: " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
#endif

	if (status == PGRES_TUPLES_OK) {
		if (resultFormat == FORMAT_TEXT) {
			answer = gettexttable();
		} else if (resultFormat == FORMAT_BINARY) {
			answer = getbinarytable();
		} else {
			answer = "UNKNOWN FORMAT";
		}
	} else if (status == PGRES_COMMAND_OK) {
		answer = "OK";
	} else {
		std::cerr << "ERROR: " << PQresultErrorMessage(m_pRes);
	}

	return answer;
}

std::string PGanswer::gettexttable(void) const
{
	std::string answer = "";

	const int ntuples = PQntuples(m_pRes);
	const int nfields = PQnfields(m_pRes);

	for (int col = 0; col < nfields; ++col) {
		char *value = PQfname(m_pRes, col);

		answer += (value ? value : "NULL");

		if (col < (nfields - 1)) {
			answer += ",";
		} else if (0 < ntuples) {
			answer += "\n";
		}
	}

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
}

std::string PGanswer::getbinarytable(void) const
{
	std::string answer = "BINARY DATA";  // TODO: Implement binary data retrieval if needed

	return answer;
}


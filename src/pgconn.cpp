
#include "pgconn.hpp"

PGconnection::PGconnection(void)
:	DBconnection(),
	m_pConn(NULL)
{
}

PGconnection::PGconnection(const std::string &connectstr, const bool blocking)
:	DBconnection(connectstr, blocking),
	m_pConn(NULL)
{
}

PGconnection::PGconnection(const char *conninfo, const bool blocking)
:	DBconnection(conninfo, blocking),
	m_pConn(NULL)
{
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconnection::PGconnection(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
:	DBconnection(keys, vals, blocking, expand_dbname),
	m_pConn(NULL)
{
}

PGconnection::~PGconnection(void)
{
}

bool PGconnection::connectdb(const char *conninfo, const bool blocking)
{
	PGconn *conn = NULL;

	if (blocking) {
		conn = PQconnectdb(conninfo);
	} else {
		conn = PQconnectStart(conninfo);
	}

	this->m_pConn = conn;

	return (conn != NULL);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
bool PGconnection::connectdb(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	PGconn *conn = NULL;

	PGconnection::dumpconninfo(keys, vals);

	if (blocking) {
		conn = PQconnectdbParams(keys, vals, expand_dbname);
	} else {
		conn = PQconnectStartParams(keys, vals, expand_dbname);
	}

	this->m_pConn = conn;

	return (conn != NULL);
}

bool PGconnection::connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	static const char *tty = NULL;
	PGconn *conn = NULL;

	if (login == NULL) {
		conn = PQsetdb(host, port, options, tty, dbName);
	} else {
		conn = PQsetdbLogin(host, port, options, tty, dbName, login, pwd);
	}

	this->m_pConn = conn;

	return (conn != NULL);
}

bool PGconnection::check(void)
{
	ConnStatusType status;
	PGresult      *res;

	status = PQstatus(this->m_pConn);

	std::cout << this->status() << std::endl;

	/* Check to see that the backend connection was successfully made */
	if (status != CONNECTION_OK) {
		std::cerr << PQerrorMessage(m_pConn) << std::endl;

                this->exit_nicely();

		return false;
        }

	/* Set always-secure search path, so malicious users can't take control. */
	res = PQexec(m_pConn, "SELECT pg_catalog.set_config('search_path', '', false)");

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cerr << "SET failed: " << PQerrorMessage(m_pConn) << std::endl;

		PQclear(res);

		this->exit_nicely();

		return false;
	}

	PQclear(res);

	return true;
}

void PGconnection::disconnect(void)
{
	if (m_pConn != NULL) {
		PQfinish(m_pConn);

		m_pConn = NULL;
	}
}

std::string PGconnection::status(void) const
{
	std::string ret;

	switch(PQstatus(m_pConn))
	{
	case CONNECTION_STARTED:
		ret = "Connecting...";
		break;

	case CONNECTION_MADE:
		ret = "Connected to server...";
		break;

	default:
		ret = "Connecting...";
	}

	return ret;
}

void PGconnection::dumpoptions(void) const
{
	PGconnection::dumpoptionarr(m_pConn);
}

PostgresPollingStatusType PGconnection::poll(void)
{
	return PQconnectPoll(m_pConn);
}

void PGconnection::dumpoptionarr(void)
{
	PQconninfoOption *options = PQconndefaults();

	PGconnection::dumpoptions(options);
}

void PGconnection::dumpoptionarr(PGconn *conn)
{
	PQconninfoOption *options = PQconninfo(conn);

	PGconnection::dumpoptions(options);
}

void PGconnection::dumpoptionarr(char *conninfo, char **errmsg)
{
	PQconninfoOption *options = PQconninfoParse(conninfo, errmsg);

	PGconnection::dumpoptions(options);

	if (errmsg != NULL) {
		std::cerr << *errmsg << std::endl;

		PQfreemem(*errmsg);
	}
}

void PGconnection::dumpoptions(PQconninfoOption *options)
{
	if (options == NULL) {
		// TODO
	} else {
		for (int index = 0; options[index].keyword != NULL; ++index) {
			PQconninfoOption &option = options[index];

			std::cout << option.keyword << " = ";

			if (! *option.dispchar) {
				std::cout << option.val;
			} else if (*option.dispchar == '*') {
				std::cout << "***";
			} else {
				std::cout << "???";
			}

			std::cout << " (" << option.keyword << option.envvar << '|' << option.compiled << ") "
				<< option.label << '[' << option.dispsize << ']' << std::endl;
		}

		PQconninfoFree(options);
	}
}

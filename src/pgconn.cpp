
#include "pganswer.hpp"

#include "pgconn.hpp"

PGconnection::PGconnection(void)
:	DBconnection(),
	m_pConn(nullptr)
{
}

PGconnection::PGconnection(const std::string &connectstr, const bool blocking)
:	DBconnection(connectstr, blocking),
	m_pConn(nullptr)
{
}

PGconnection::PGconnection(const char *conninfo, const bool blocking)
:	DBconnection(conninfo, blocking),
	m_pConn(nullptr)
{
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconnection::PGconnection(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
:	DBconnection(keys, vals, blocking, expand_dbname),
	m_pConn(nullptr)
{
}

PGconnection::~PGconnection(void)
{
}

bool PGconnection::connectdb(const char *conninfo, const bool blocking)
{
	PGconn *conn = nullptr;

	if (blocking) {
		conn = PQconnectdb(conninfo);
	} else {
		conn = PQconnectStart(conninfo);
	}

	this->m_pConn = conn;

	return (conn != nullptr);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
bool PGconnection::connectdb(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	PGconn *conn = nullptr;

	PGconnection::dumpconninfo(keys, vals);

	if (blocking) {
		conn = PQconnectdbParams(keys, vals, expand_dbname);
	} else {
		conn = PQconnectStartParams(keys, vals, expand_dbname);
	}

	this->m_pConn = conn;

	return (conn != nullptr);
}

bool PGconnection::connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	static const char *tty = nullptr;
	PGconn *conn = nullptr;

	if (login == nullptr) {
		conn = PQsetdb(host, port, options, tty, dbName);
	} else {
		conn = PQsetdbLogin(host, port, options, tty, dbName, login, pwd);
	}

	this->m_pConn = conn;

	return (conn != nullptr);
}

bool PGconnection::checkconnect(void)
{
	std::cout << this->status() << std::endl;

	/* Set always-secure search path, so malicious users can't take control. */
	const std::string answer = this->getanswer("SELECT pg_catalog.set_config('search_path', '', false)", "SET failed");

	if (answer == DBconnection::m_error) {
		return false;
	}

	return true;
}

bool PGconnection::check(void)
{
	ConnStatusType status = PQstatus(this->m_pConn);

	/* Check to see that the backend connection was successfully made */
	if (status != CONNECTION_OK) {
		std::cerr << PQerrorMessage(m_pConn);

		this->exit_nicely();

		return false;
	}

	return true;
}

void PGconnection::disconnect(const bool force)
{
	if ((m_pConn != nullptr) && (force || this->getDisconnectOnError() || this->getExitOnError())) {
		PQfinish(m_pConn);

		m_pConn = nullptr;
	}
}

const std::string PGconnection::status(void) const
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

const DBanswer *PGconnection::exec(const char *command, const char *errmsg, const DBparameterFormat /*resultFormat*/)
{
	if (! this->check()) {
		return nullptr;
	}

	PGresult *res = PQexec(this->m_pConn, command);
	ExecStatusType status = PQresultStatus(res);

	if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
		std::cerr << errmsg << ": " << PQerrorMessage(m_pConn);

		PQclear(res);

		this->exit_nicely();

		return nullptr;
	}

	return new PGanswer(res);
}

const DBanswer *PGconnection::exec(const char *command, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	if (! this->check()) {
		return nullptr;
	}

	PGresult *res = PQexecParams(this->m_pConn, command, param.count(), param.types(), param.values(), param.lengths(), param.formats(), resultFormat);
	ExecStatusType status = PQresultStatus(res);

	if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
		std::cerr << errmsg << ": " << PQerrorMessage(m_pConn);

		PQclear(res);

		this->exit_nicely();

		return nullptr;
	}

	return new PGanswer(res);
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

	if (errmsg != nullptr) {
		std::cerr << *errmsg << std::endl;

		PQfreemem(*errmsg);
	}
}

void PGconnection::dumpoptions(PQconninfoOption *options)
{
	if (options == nullptr) {
		// TODO
	} else {
		for (int index = 0; options[index].keyword != nullptr; ++index) {
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


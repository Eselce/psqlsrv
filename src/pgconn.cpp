
#include <iostream>

#include "pgconn.hpp"

PGconnection::PGconnection(void)
:	m_pConn(NULL)
{
}

PGconnection::PGconnection(const std::string &connectstr, const bool blocking)
:	m_pConn(NULL)
{
	this->connect(connectstr, blocking);
}

PGconnection::PGconnection(const char *conninfo, const bool blocking)
:	m_pConn(NULL)
{
	this->connect(conninfo, blocking);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconnection::PGconnection(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
:	m_pConn(NULL)
{
	this->connect(keys, vals, blocking, expand_dbname);
}

PGconnection::~PGconnection(void)
{
	this->disconnect();
}

PGconn *PGconnection::connect(const std::string &connectstr, const bool blocking)
{
	const char *conninfo = connectstr.c_str();

	return this->connect(conninfo, blocking);
}

PGconn *PGconnection::connect(const char *conninfo, const bool blocking)
{
	static const char *host = "localhost";
	static const char *hostaddr = "127.0.0.1";
	static const char *port = "5432";
	static const char *dbname = "loges";
	static const char *user = "loges";
	static const char *password = "";
//	static const char *passfile = "~/.pgpass";
	static const char *options = "";
	static const char *keys[] = { "host", "hostaddr", "port", "dbname", "user", /*"password", "passfile",*/ NULL };
	static const char *vals[] = { host, hostaddr, port, dbname, user, /*password, passfile,*/ NULL };

	const bool defaultconn = ((conninfo == NULL) || (! *conninfo));
	PGconn *conn = NULL;

	this->disconnect();

	if (defaultconn) {
		if (conninfo != NULL) {
			static const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)

			conn = this->connectdb(keys, vals, blocking, expand_dbname);
		} else {
			conn = this->connectdb(host, port, options, dbname, user, password);
		}
	} else {
		conn = this->connectdb(conninfo, blocking);
	}

	this->m_pConn = conn;

	return conn;
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconn *PGconnection::connect(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	this->disconnect();

	this->dumpoptions();

	PGconn *conn = this->connectdb(keys, vals, blocking, expand_dbname);

	this->m_pConn = conn;

	this->dumpoptions();

	return conn;
}

PGconn *PGconnection::connect(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	this->disconnect();

	PGconn *conn = this->connectdb(host, port, options, dbName, login, pwd);

	this->m_pConn = conn;

	return conn;
}

PGconn *PGconnection::connectdb(const char *conninfo, const bool blocking)
{
	PGconn *conn = NULL;

	if (blocking) {
		conn = PQconnectdb(conninfo);
	} else {
		conn = PQconnectStart(conninfo);
	}

	return conn;
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconn *PGconnection::connectdb(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	PGconn *conn = NULL;

	PGconnection::dumpconninfo(keys, vals);

	if (blocking) {
		conn = PQconnectdbParams(keys, vals, expand_dbname);
	} else {
		conn = PQconnectStartParams(keys, vals, expand_dbname);
	}

	return conn;
}

PGconn *PGconnection::connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	static const char *tty = NULL;
	PGconn *conn = NULL;

	if (login == NULL) {
		PQsetdb(host, port, options, tty, dbName);
	} else {
		PQsetdbLogin(host, port, options, tty, dbName, login, pwd);
	}

	return conn;
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

void PGconnection::dumpoptions(void) const
{
	PGconnection::dumpoptionarr(m_pConn);
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

void PGconnection::dumpconninfo(const char * const *keys, const char * const *vals)
{
	for (int index = 0; (keys[index] != NULL) && *keys[index]; ++index) {
		std::cout << keys[index] << " = " << vals[index] << std::endl;
	}
}

void PGconnection::exit_nicely(void)
{
	this->disconnect();

	exit(1);
}


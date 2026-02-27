
#include <iostream>

#include "pg_type.h"

#include "pganswer.hpp"

#include "pgrecset.hpp"

#include "pgconn.hpp"

PGconnection::PGconnection(void)
:	DBconnection(),
	m_pConn(nullptr)
{
}

PGconnection::PGconnection(const std::string &connectstr, const bool blocking, const int verboselevel)
:	DBconnection(connectstr, blocking, verboselevel),
	m_pConn(nullptr)
{
}

PGconnection::PGconnection(const char *conninfo, const bool blocking, const int verboselevel)
:	DBconnection(conninfo, blocking, verboselevel),
	m_pConn(nullptr)
{
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconnection::PGconnection(const char **keys, const char **vals, const bool blocking, const int verboselevel, const int expand_dbname)
:	DBconnection(keys, vals, blocking, verboselevel, expand_dbname),
	m_pConn(nullptr)
{
}

PGconnection::~PGconnection(void)
{
}

void PGconnection::setverbose(const int verboselevel)
{
	if (! this->check()) {
		return;
	}

	const PGVerbosity verbosity = static_cast<PGVerbosity>(verboselevel);

#if defined(_DEBUG)
    if (this->getVerbose()) {
        std::clog << "Setting postgreSQL connection to verbose level : " << verboselevel
					 << " [" << PGVerbosityName[verboselevel] << "]" << std::endl;
    }
#endif

	PQsetErrorVerbosity(this->m_pConn, verbosity);
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
bool PGconnection::connectdb(const char **keys, const char **vals, const bool blocking, const int expand_dbname)
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

PGconn *PGconnection::getPGconn(void) const
{
	return this->m_pConn;
}

bool PGconnection::checkconnect(void)
{
	std::cout << this->statusconnect() << std::endl;

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
        this->printerror();

#if defined(_DEBUG)
        std::clog << "check() got error status: " << status << " [" << ConnStatusTypeName[status] << "]" << std::endl;
#endif

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

std::string PGconnection::statusconnect(void) const
{
	ConnStatusType status = PQstatus(m_pConn);
	std::string ret;

#if defined(_DEBUG)
        if (this->getVerbose()) {

            std::clog << "Connection status: " << status << " [" << ConnStatusTypeName[status] << "]" << std::endl;
        }
#endif

	switch(status)
	{
	case CONNECTION_STARTED:
		ret = "Connecting...";
		break;

	case CONNECTION_MADE:
		ret = "Connected to server...";
		break;

	case CONNECTION_OK:
		ret = "OK";
		break;

	default:
		ret = "Connecting...";
		break;
	}

	return ret;
}

const DBanswer *PGconnection::exec(const char *command, const char *errmsg, [[maybe_unused]] const DBparameterFormat resultFormat)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
        std::clog << "Executing: " << command << std::endl;
    }
#endif

	PGresult *res = PQexec(this->m_pConn, command);

	return this->checkresult(res, "PQexec", "Execute command (status)", errmsg);
}

const DBanswer *PGconnection::exec(const char *command, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
        std::clog << "Executing: " << command << " [" << param.count() << "] to " << resultFormat << std::endl;
    }
#endif

	PGresult *res = PQexecParams(this->m_pConn, command, param.count(), param.types(), param.values(), param.lengths(), param.formats(), resultFormat);

	return this->checkresult(res, "PQexecParams", "Execute command (status)", errmsg);
}

const DBanswer *PGconnection::exec(const DBstatement *stmt, const char *errmsg, [[maybe_unused]] const DBparameterFormat resultFormat)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
		std::string stmtText = ((stmt == nullptr) ? (" (" + std::string((stmt->getName() == nullptr) ? "null" : stmt->getName()) + ")") : " (null)");

		std::clog << "Executing: " << stmt->getCommand() << stmtText << std::endl;
    }
#endif

	PGresult *res = PQexec(this->m_pConn, stmt->getCommand());
	std::string logmsg = "Execute statement ";

	logmsg += stmt->getName();
	logmsg += " (status)";

	return this->checkresult(res, "PQexec", logmsg.c_str(), errmsg);
}

const DBanswer *PGconnection::exec(const DBstatement *stmt, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
		std::string stmtText = ((stmt != nullptr) ? (" (" + std::string((stmt->getName() == nullptr) ? "null" : stmt->getName()) + ")") : " (null)");

        std::clog << "Executing: " << stmt->getCommand() << stmtText << " [" << param.count() << "] to " << resultFormat << std::endl;
    }
#endif

	PGresult *res = PQexecPrepared(this->m_pConn, stmt->getName(), param.count(), param.values(), param.lengths(), param.formats(), resultFormat);
	std::string logmsg = "Execute statement ";

	logmsg += stmt->getName();
	logmsg += " (status)";

	return this->checkresult(res, "PQexecPrepared", logmsg.c_str(), errmsg);
}

DBrecordset *PGconnection::query(const char *command, const char *stmtName)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
		std::string stmtText = (" (" + std::string((stmtName == nullptr) ? "null" : stmtName) + ")");

		std::clog << "Query: " << command << stmtText << std::endl;
    }
#endif

	DBrecordset *recset = new PGrecordset(this, command, 0, nullptr, stmtName);

	return recset;
}

DBrecordset *PGconnection::query(const char *command, const DBparameter &param, const char *stmtName)
{
	if (! this->check()) {
		return nullptr;
	}

#if defined(_DEBUG)
    if (this->getVerbose()) {
		std::string stmtText = (" (" + std::string((stmtName == nullptr) ? "null" : stmtName) + ")");

        std::clog << "Query: " << command << stmtText << " [" << param.count() << "]" << std::endl;
    }
#endif

	DBrecordset *recset = new PGrecordset(this, command, param.count(), param.types(), stmtName);
	
	return recset;
}

const DBanswer *PGconnection::checkresult(PGresult *res, const char *funName, const char *logmsg, const char *errmsg)
{
	ExecStatusType status = PQresultStatus(res);

#if defined(_DEBUG)
    if (this->getVerbose()) {
        std::clog << logmsg << ": " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
    }
#endif

	if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
        this->printerror(errmsg);

		PQclear(res);

#if defined(_DEBUG)
		if (this->getVerbose()) {
			std::clog << funName << " returned error status: " << status 
						<< " [" << ExecStatusTypeName[status] << "]" << std::endl;
			std::clog << "Cleared result: " << this << std::endl;
		}
#endif

		this->exit_nicely();

		return nullptr;
	}

	return new PGanswer(res);
}

std::string PGconnection::geterrorstring(void) const
{
	// This errorMessage is either empty or a (multi line) string
	// held by PQ with newline at the end. Don't free it! PQfinish does it.
	char *errorMessage = PQerrorMessage(this->getPGconn());
	std::string ret(errorMessage);

	return ret;
}

void PGconnection::freestmt(const char *stmtname)
{
    // Freeing statement quick and dirty, there may be no API function for it...
    std::string deallocate = "DEALLOCATE PREPARE ";

    deallocate += stmtname;

    this->getanswer(deallocate.c_str()); 
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


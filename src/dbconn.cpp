
#include <iostream>

#include "postgresql/pg_config.h"

#include "dbconn.hpp"

const char *DBconnection::m_error = "ERROR";

DBconnection::DBconnection(void)
:	NiceService(false, false, false, false)
{
}

DBconnection::DBconnection(const std::string &connectstr, const bool blocking)
:	NiceService(false, false, blocking, false)
{
	this->connect(connectstr, this->getBlocking());
}

DBconnection::DBconnection(const char *conninfo, const bool blocking)
:	NiceService(false, false, blocking, false)
{
	this->connect(conninfo, this->getBlocking());
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection::DBconnection(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
:	NiceService(false, blocking, false)
{
	this->connect(keys, vals, this->getBlocking(), expand_dbname);
}

DBconnection::~DBconnection(void)
{
}

DBconnection *DBconnection::connect(const std::string &connectstr, const bool blocking)
{
	const char *conninfo = connectstr.c_str();

	return this->connect(conninfo, blocking);
}

DBconnection *DBconnection::connect(const char *conninfo, const bool blocking)
{
	static const char *host = "localhost";
	static const char *hostaddr = "127.0.0.1";
	static const char *port = DEF_PGPORT_STR; // "5432"
	static const char *dbname = "loges";
	static const char *user = "loges";
	static const char *password = "";
//	static const char *passfile = "~/.PGpass";
	static const char *options = "";
	static const char *keys[] = { "host", "hostaddr", "port", "dbname", "user", /*"password", "passfile",*/ nullptr };
	static const char *vals[] = { host, hostaddr, port, dbname, user, /*password, passfile,*/ nullptr };

	const bool defaultconn = ((conninfo == nullptr) || (! *conninfo));
	bool connected = false;

	this->disconnect();

	if (defaultconn) {
		if (conninfo != nullptr) {
			static const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)

			connected = this->connectdb(keys, vals, blocking, expand_dbname);
		} else {
			connected = this->connectdb(host, port, options, dbname, user, password);
		}
	} else {
		connected = this->connectdb(conninfo, blocking);
	}

	return (connected ? this : nullptr);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection *DBconnection::connect(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	this->disconnect();

	this->dumpoptions();

	bool connected = this->connectdb(keys, vals, blocking, expand_dbname);

	this->dumpoptions();

	return (connected ? this : nullptr);
}

DBconnection *DBconnection::connect(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	this->disconnect();

	bool connected = this->connectdb(host, port, options, dbName, login, pwd);

	return (connected ? this : nullptr);
}

const std::string DBconnection::getanswer(const char *command, const char *errmsg)
{
	const DBanswer *answ = this->exec(command, errmsg);

	const std::string answer = this->answerstring(answ);

#if defined(_DEBUG)
	if (this->getVerbose()) {
		std::clog << "[" << this << "] " << command << " -> " << answer << std::endl;
	}
#endif

	if (answ != nullptr) {
		delete answ;
	}

	return answer;
}

const std::string DBconnection::getanswer(const char *command, const DBparameter &param, const char *errmsg)
{
	const DBanswer *answ = this->exec(command, param, errmsg);

	const std::string answer = this->answerstring(answ);

#if defined(_DEBUG)
	if (this->getVerbose()) {
		std::clog << "[" << this << "] " << command << " -> " << answer << std::endl;
	}
#endif

	if (answ != nullptr) {
		delete answ;
	}

	return answer;
}

const std::string DBconnection::answerstring(const DBanswer *answ)
{
	if (answ == nullptr) {
		return DBconnection::m_error;
	}

	return answ->getanswer();
}

void DBconnection::dumpconninfo(const char * const *keys, const char * const *vals)
{
	for (int index = 0; (keys[index] != nullptr) && *keys[index]; ++index) {
		std::cout << keys[index] << " = " << vals[index] << std::endl;
	}
}


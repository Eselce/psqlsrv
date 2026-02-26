
#include <iostream>

#include "dbconn.hpp"

static const bool verbose = ! false;

const char *cmdErrorMsg = "Command failed";

const char *DBconnection::m_error = "ERROR";

DBconnection::DBconnection(void)
:	NiceService(false, false, false, verbose)
{
}

DBconnection::DBconnection(const std::string &connectstr, const bool blocking, const int verboselevel)
:	NiceService(false, false, blocking, verbose)
{
	this->connect(connectstr, this->getBlocking(), verboselevel);
}

DBconnection::DBconnection(const char *conninfo, const bool blocking, const int verboselevel)
:	NiceService(false, false, blocking, verbose)
{
	this->connect(conninfo, this->getBlocking(), verboselevel);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection::DBconnection(const char **keys, const char **vals, const bool blocking, const int verboselevel, const int expand_dbname)
:	NiceService(false, blocking, verbose)
{
	this->connect(keys, vals, this->getBlocking(), verboselevel, expand_dbname);
}

DBconnection::~DBconnection(void)
{
}

DBconnection *DBconnection::connect(const std::string &connectstr, const bool blocking, const int verboselevel)
{
	const char *conninfo = connectstr.c_str();

	DBconnection *conn = this->connect(conninfo, blocking);

	if (conn != nullptr) {
		this->setverbose(verboselevel);
	}

	return conn;
}

DBconnection *DBconnection::connect(const char *conninfo, const bool blocking, const int verboselevel)
{
	static const char *host = "localhost";
	static const char *hostaddr = "127.0.0.1";
	static const char *port = "5432" /*DEF_PGPORT_STR*/; // "5432"
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

	if (connected) {
		this->setverbose(verboselevel);
	}

	return (connected ? this : nullptr);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection *DBconnection::connect(const char **keys, const char **vals, const bool blocking, const int verboselevel, const int expand_dbname)
{
	this->disconnect();

	this->dumpoptions();

	bool connected = this->connectdb(keys, vals, blocking, expand_dbname);

	this->dumpoptions();

	if (connected) {
		this->setverbose(verboselevel);
	}

	return (connected ? this : nullptr);
}

DBconnection *DBconnection::connect(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd, const int verboselevel)
{
	this->disconnect();

	bool connected = this->connectdb(host, port, options, dbName, login, pwd);

	if (connected) {
		this->setverbose(verboselevel);
	}

	return (connected ? this : nullptr);
}

std::string DBconnection::getanswer(const char *command, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(command, errmsg, resultFormat);

	const std::string answer = this->getanswer(answ, resultFormat);

	return answer;
}

std::string DBconnection::getanswer(const char *command, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(command, param, errmsg, resultFormat);

	const std::string answer = this->getanswer(answ, resultFormat);

	return answer;
}

std::string DBconnection::getanswer(const DBstatement *stmt, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(stmt, errmsg, resultFormat);

	const std::string answer = this->getanswer(answ, resultFormat);

	return answer;
}

std::string DBconnection::getanswer(const DBstatement *stmt, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(stmt, param, errmsg, resultFormat);

	const std::string answer = this->getanswer(answ, resultFormat);

	return answer;
}

std::string DBconnection::getanswer(const DBanswer *answ, const DBparameterFormat resultFormat)
{
	const std::string answer = ((resultFormat == FORMAT_TEXT)
									? DBconnection::answerstring(answ)
									: DBconnection::binaryanswer(answ));

#if defined(_DEBUG)
	if (this->getVerbose()) {
		std::clog << "[" << this << "] " << " -> " << answer << std::endl;
	}
#endif

	if (answ != nullptr) {
		delete answ;

#if defined(_DEBUG)
		if (this->getVerbose()) {
			std::clog << "Deleted answer: " << this << std::endl;
		}
#endif
	}

	return answer;
}

std::string DBconnection::answerstring(const DBanswer *answ)
{
	if (answ == nullptr) {
		return DBconnection::m_error;
	}

	return answ->getanswer(FORMAT_TEXT);
}

std::string DBconnection::binaryanswer(const DBanswer *answ)
{
	if (answ == nullptr) {
		return DBconnection::m_error;
	}

	return answ->getanswer(FORMAT_BINARY);
}

void DBconnection::printerror(const char *errmsg, std::ostream &ost) const
{
	std::string errorstr = this->geterrorstring();

	if (! errorstr.empty()) {
		ost << errmsg << ": " << errorstr;
	}
}

void DBconnection::dumpconninfo(const char **keys, const char **vals)
{
	for (int index = 0; (keys[index] != nullptr) && *keys[index]; ++index) {
		std::cout << keys[index] << " = " << vals[index] << std::endl;
	}
}


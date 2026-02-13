
#include <iostream>

#include "dbconn.hpp"

DBconnection::DBconnection(void)
{
}

DBconnection::DBconnection(const std::string &connectstr, const bool blocking)
{
	this->connect(connectstr, blocking);
}

DBconnection::DBconnection(const char *conninfo, const bool blocking)
{
	this->connect(conninfo, blocking);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection::DBconnection(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	this->connect(keys, vals, blocking, expand_dbname);
}

DBconnection::~DBconnection(void)
{
	this->disconnect();
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
	static const char *port = "5432";
	static const char *dbname = "loges";
	static const char *user = "loges";
	static const char *password = "";
//	static const char *passfile = "~/.PGpass";
	static const char *options = "";
	static const char *keys[] = { "host", "hostaddr", "port", "dbname", "user", /*"password", "passfile",*/ NULL };
	static const char *vals[] = { host, hostaddr, port, dbname, user, /*password, passfile,*/ NULL };

	const bool defaultconn = ((conninfo == NULL) || (! *conninfo));
	bool connected = false;

	this->disconnect();

	if (defaultconn) {
		if (conninfo != NULL) {
			static const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)

			connected = this->connectdb(keys, vals, blocking, expand_dbname);
		} else {
			connected = this->connectdb(host, port, options, dbname, user, password);
		}
	} else {
		connected = this->connectdb(conninfo, blocking);
	}

	return (connected ? this : NULL);
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
DBconnection *DBconnection::connect(const char * const *keys, const char * const *vals, const bool blocking, const int expand_dbname)
{
	this->disconnect();

	this->dumpoptions();

	bool connected = this->connectdb(keys, vals, blocking, expand_dbname);

	this->dumpoptions();

	return (connected ? this : NULL);
}

DBconnection *DBconnection::connect(const char *host, const char *port, const char *options, const char *dbName, const char *login, const char *pwd)
{
	this->disconnect();

	bool connected = this->connectdb(host, port, options, dbName, login, pwd);

	return (connected ? this : NULL);
}

void DBconnection::disconnect(void)
{
}

void DBconnection::dumpconninfo(const char * const *keys, const char * const *vals)
{
	for (int index = 0; (keys[index] != NULL) && *keys[index]; ++index) {
		std::cout << keys[index] << " = " << vals[index] << std::endl;
	}
}

void DBconnection::exit_nicely(void)
{
	this->disconnect();

	std::exit(1);
}

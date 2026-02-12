
#pragma once

#include <iostream>

#include "postgresql/libpq-fe.h"

class PGconnection
{
public:
	PGconnection(void);

	PGconnection(const std::string &connectstr, const bool blocking = false);

	PGconnection(const char *conninfo, const bool blocking = false);

	PGconnection(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual ~PGconnection(void);

public:
	PGconn *connect(const std::string &connectstr = "", const bool blocking = false);

	PGconn *connect(const char *conninfo, const bool blocking = false);

	PGconn *connect(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	PGconn *connect(const char *host, const char *port, const char *options, const char *dbName, const char *login = NULL, const char *pwd = NULL);

	bool check(void);

	void disconnect(void);

	std::string status(void) const;

	PostgresPollingStatusType poll(void);

	void dumpoptions(void) const;

protected:
	static PGconn *connectdb(const char *conninfo, const bool blocking = false);

	static PGconn *connectdb(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	static PGconn *connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = NULL, const char *pwd = NULL);

	static void dumpoptionarr(void);

	static void dumpoptionarr(PGconn *conn);

	static void dumpoptionarr(char *conninfo, char **errmsg);

	static void dumpoptions(PQconninfoOption *options);

	static void dumpconninfo(const char * const *keys, const char * const *vals);

	void exit_nicely(void);

protected:
	PGconn *m_pConn;
};


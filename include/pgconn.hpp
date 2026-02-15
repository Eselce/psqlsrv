
#pragma once

#include <iostream>

#include "postgresql/libpq-fe.h"

#include "dbconn.hpp"

class PGconnection : public DBconnection
{
public:
	PGconnection(void);

	PGconnection(const std::string &connectstr, const bool blocking = false);

	PGconnection(const char *conninfo, const bool blocking = false);

	PGconnection(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual ~PGconnection(void);

public:
	virtual bool checkconnect(void);

	virtual bool check(void);

	virtual void disconnect(void);

	virtual std::string status(void) const;

	virtual DBanswer *exec(const char *command, const char *errmsg = "Command failed");

	virtual DBanswer *exec(const char *command, const DBparameter &param, const char *errmsg = "Command failed");

	virtual void dumpoptions(void) const;

protected:
	PostgresPollingStatusType poll(void);

	virtual bool connectdb(const char *conninfo, const bool blocking = false);

	virtual bool connectdb(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = NULL, const char *pwd = NULL);

	static void dumpoptionarr(void);

	static void dumpoptionarr(PGconn *conn);

	static void dumpoptionarr(char *conninfo, char **errmsg);

	static void dumpoptions(PQconninfoOption *options);

protected:
	PGconn *m_pConn;
};


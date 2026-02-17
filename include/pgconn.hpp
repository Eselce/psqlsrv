
#pragma once

#include <iostream>

#include "postgresql/libpq-fe.h"

#include "dbconn.hpp"

static const char *cmdErrorMsg = "Command failed";

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

	virtual void disconnect(const bool force = true);

	virtual const std::string status(void) const;

	virtual const DBanswer *exec(const char *command, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual const DBanswer *exec(const char *command, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual void dumpoptions(void) const;

protected:
	PostgresPollingStatusType poll(void);

	virtual bool connectdb(const char *conninfo, const bool blocking = false);

	virtual bool connectdb(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr);

	static void dumpoptionarr(void);

	static void dumpoptionarr(PGconn *conn);

	static void dumpoptionarr(char *conninfo, char **errmsg);

	static void dumpoptions(PQconninfoOption *options);

protected:
	PGconn *m_pConn;
};


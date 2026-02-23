
#pragma once

#include <iostream>

#include "pg_type.h"

#include "dbconn.hpp"

class PGconnection : public DBconnection
{
public:
	PGconnection(void);

	PGconnection(const std::string &connectstr, const bool blocking = false, const int verboselevel = PQERRORS_VERBOSE);

	PGconnection(const char *conninfo, const bool blocking = false, const int verboselevel = PQERRORS_VERBOSE);

	PGconnection(const char **keys,
					const char **vals,
					const bool blocking = false,
					const int verboselevel = PQERRORS_VERBOSE,
					const int expand_dbname = 0);

	virtual ~PGconnection(void) override;

public:
	virtual void setverbose(const int verboselevel = PQERRORS_DEFAULT) override;

	virtual bool checkconnect(void) override;

	virtual bool check(void) override;

	virtual void disconnect(const bool force = true) override;

	virtual std::string status(void) const override;

	virtual const DBanswer *exec(const char *command, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) override;

	virtual const DBanswer *exec(const char *command, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) override;

	virtual const DBanswer *exec(const DBstatement *stmt, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) override;

	virtual const DBanswer *exec(const DBstatement *stmt, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) override;

	virtual DBrecordset *query(const char *command, const char *stmtName = nullptr) override;

	virtual DBrecordset *query(const char *command, const DBparameter &param, const char *stmtName = nullptr) override;

	virtual PGconn *getPGconn(void) const;

	virtual void dumpoptions(void) const override;

protected:
	PostgresPollingStatusType poll(void);

	virtual bool connectdb(const char *conninfo, const bool blocking = false) override;

	virtual bool connectdb(const char **keys, const char **vals, const bool blocking = false, const int expand_dbname = 0) override;

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr) override;

	static void dumpoptionarr(void);

	static void dumpoptionarr(PGconn *conn);

	static void dumpoptionarr(char *conninfo, char **errmsg);

	static void dumpoptions(PQconninfoOption *options);

protected:
	PGconn *m_pConn;
};


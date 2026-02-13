
#pragma once

#include <string>

class DBconnection
{
public:
	DBconnection(void);

	DBconnection(const std::string &connectstr, const bool blocking = false);

	DBconnection(const char *conninfo, const bool blocking = false);

	DBconnection(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual ~DBconnection(void);

public:
	virtual DBconnection *connect(const std::string &connectstr = "", const bool blocking = false);

	virtual DBconnection *connect(const char *conninfo, const bool blocking = false);

	virtual DBconnection *connect(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual DBconnection *connect(const char *host, const char *port, const char *options, const char *dbName, const char *login = NULL, const char *pwd = NULL);

	virtual void disconnect(void);

	virtual bool check(void) = 0;

	virtual std::string status(void) const = 0;

	virtual void dumpoptions(void) const = 0;

protected:
	virtual bool connectdb(const char *conninfo, const bool blocking = false) = 0;

	virtual bool connectdb(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0) = 0;

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = NULL, const char *pwd = NULL) = 0;

	static void dumpconninfo(const char * const *keys, const char * const *vals);

	virtual void exit_nicely(void);
};

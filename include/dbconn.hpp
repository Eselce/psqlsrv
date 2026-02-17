
#pragma once

#include <string>

#include "dbanswer.hpp"

#include "nicesvc.hpp"

class DBconnection : public NiceService
{
public:
	DBconnection(void);

	DBconnection(const std::string &connectstr, const bool blocking = false);

	DBconnection(const char *conninfo, const bool blocking = false);

	DBconnection(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual ~DBconnection(void) override;

public:
	virtual DBconnection *connect(const std::string &connectstr = "", const bool blocking = false);

	virtual DBconnection *connect(const char *conninfo, const bool blocking = false);

	virtual DBconnection *connect(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0);

	virtual DBconnection *connect(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr);

	virtual bool checkconnect(void) = 0;

	virtual bool check(void) = 0;

	virtual const std::string status(void) const = 0;

	virtual const DBanswer *exec(const char *command, const char *errmsg = "Command failed", const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual const DBanswer *exec(const char *command, const DBparameter &param, const char *errmsg = "Command failed", const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual const std::string getanswer(const char *command, const char *errmsg = "Command failed", const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual const std::string getanswer(const char *command, const DBparameter &param, const char *errmsg = "Command failed", const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual void dumpoptions(void) const = 0;

protected:
	virtual bool connectdb(const char *conninfo, const bool blocking = false) = 0;

	virtual bool connectdb(const char * const *keys, const char * const *vals, const bool blocking = false, const int expand_dbname = 0) = 0;

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr) = 0;

	static const std::string answerstring(const DBanswer *answ);

	static const std::string binaryanswer(const DBanswer *answ);

	static void dumpconninfo(const char * const *keys, const char * const *vals);

public:
	static const char *m_error;
};


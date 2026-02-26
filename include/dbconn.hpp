
#pragma once

#include <iostream>

#include <string>

#include "dbanswer.hpp"

#include "dbrecset.hpp"

#include "nicesvc.hpp"

class DBconnection : public NiceService
{
public:
	DBconnection(void);

	DBconnection(const std::string &connectstr, const bool blocking = false, const int verboselevel = 0);

	DBconnection(const char *conninfo, const bool blocking = false, const int verboselevel = 0);

	DBconnection(const char **keys, const char **vals, const bool blocking = false, const int verboselevel = 0, const int expand_dbname = 0);

	virtual ~DBconnection(void) override;

public:
	virtual void setverbose(const int verboselevel) = 0;

	virtual DBconnection *connect(const std::string &connectstr = "", const bool blocking = false, const int verboselevel = 0);

	virtual DBconnection *connect(const char *conninfo, const bool blocking = false, const int verboselevel = 0);

	virtual DBconnection *connect(const char **keys, const char **vals, const bool blocking = false, const int verboselevel = 0, const int expand_dbname = 0);

	virtual DBconnection *connect(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr, const int verboselevel = 0);

	virtual bool checkconnect(void) = 0;

	virtual bool check(void) = 0;

	virtual std::string statusconnect(void) const = 0;

	virtual const DBanswer *exec(const char *command, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual const DBanswer *exec(const char *command, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual const DBanswer *exec(const DBstatement *stmt, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual const DBanswer *exec(const DBstatement *stmt, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT) = 0;

	virtual std::string getanswer(const char *command, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const char *command, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const DBstatement *stmt, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const DBstatement *stmt, const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const DBanswer *answ, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual DBrecordset *query(const char *command, const char *stmtName = nullptr) = 0;

	virtual DBrecordset *query(const char *command, const DBparameter &param, const char *stmtName = nullptr) = 0;

	virtual void printerror(const char *errmsg = cmdErrorMsg, std::ostream &ost = std::cerr) const;

	virtual void freestmt(const char *stmtname) = 0;

	virtual void dumpoptions(void) const = 0;

protected:
	virtual bool connectdb(const char *conninfo, const bool blocking = false) = 0;

	virtual bool connectdb(const char **keys, const char **vals, const bool blocking = false, const int expand_dbname = 0) = 0;

	virtual bool connectdb(const char *host, const char *port, const char *options, const char *dbName, const char *login = nullptr, const char *pwd = nullptr) = 0;

	static std::string answerstring(const DBanswer *answ);

	static std::string binaryanswer(const DBanswer *answ);

	virtual std::string geterrorstring(void) const = 0;

	static void dumpconninfo(const char **keys, const char **vals);

protected:
	static const char *m_error;  // = "ERROR";
};


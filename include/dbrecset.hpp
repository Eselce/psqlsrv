
#pragma once

#include "db_type.h"

#include "dbstmt.hpp"

#include "dbanswer.hpp"

#include "dbtupel.hpp"

class DBrecordset
{
public:
	DBrecordset(DBconnection *conn,
				const char *command,
				const int nParams = 0,
				const DBparameterType *paramTypes = nullptr,
				const char *stmtName = nullptr);

	virtual ~DBrecordset(void);

	virtual void init(const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr, const char *stmtName = nullptr);

	virtual void createStatement(const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr, const char *stmtName = nullptr);

	virtual void prepare(void);

	virtual bool check(void);

	virtual const DBanswer *exec(const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual const DBanswer *exec(const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual std::string getanswer(const DBparameter &param, const char *errmsg = cmdErrorMsg, const DBparameterFormat resultFormat = FORMAT_TEXT);

	virtual const char *getStmtName(void) const;

	virtual const char *getCommand(void) const;

	virtual const DBparameterType *getParamTypes(void) const;

	virtual DBstatement *getStatement(void) const;

	virtual DBparameter *getParameter(void) const;

	virtual DBtupel *getTupel(void) const;

	virtual bool first(void);

	virtual bool last(void);

	virtual bool next(void);

	virtual bool prev(void);

	virtual bool isFirst(void) const;

	virtual bool isLast(void) const;

	virtual bool hasData(void) const;

	virtual bool isEmpty(void) const;

	virtual void reset(void);

	virtual int getCurrentRow(void) const;

	virtual int getNRows(void) const;

	virtual int getNFields(void) const;

	virtual const char *getValue(const int field, const char *defaultValue = nullptr) const;

	virtual const char *getValue(const char *fieldName, const char *defaultValue = nullptr) const;

	virtual const DBparameterType *getFieldTypes(void) const = 0;

	virtual std::string getFieldName(const int field) const = 0;

	virtual int getFieldNumber(const std::string fieldName) const = 0;

protected:
	DBconnection *m_pConn;

	DBstatement *m_pStmt;

	DBparameter *m_pParam;

	DBanswer *m_pAnswer;

	int nFields;

	int nRows;

	int currentRow;

	DBtupel *m_pTupel;
};



#pragma once

//#include "dbconn.hpp"

class DBconnection;  // forward declaration to avoid circular dependency

#include "dbstmt.hpp"

class DBrecordset
{
public:
	DBrecordset(DBconnection *conn, const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~DBrecordset(void);

	virtual void createStatement(const char *command, const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual void setStmtName(const char *stmtName);

	virtual const char *getStmtName(void) const;

protected:
	DBconnection *m_pConn;

	DBstatement *m_pStmt;
};


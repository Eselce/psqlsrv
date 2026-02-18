
#include "dbconn.hpp"

#include "dbrecset.hpp"

DBrecordset::DBrecordset(DBconnection *conn, const char *command, const int nParams, const DBparameterType *paramTypes)
:	m_pConn(conn),
    m_pStmt(nullptr)
{
    createStatement(command, nParams, paramTypes);
}

DBrecordset::~DBrecordset(void)
{
}

void DBrecordset::createStatement(const char *command, const int nParams, const DBparameterType *paramTypes)
{
    if (this->m_pStmt != nullptr) {
        delete this->m_pStmt;
    }

    this->m_pStmt = new DBstatement(command, nParams, paramTypes);
}

void DBrecordset::setStmtName(const char *stmtName)
{
    if (this->m_pStmt != nullptr) {
        this->m_pStmt->setName(stmtName);
    }
}

const char *DBrecordset::getStmtName(void) const
{
    if (this->m_pStmt != nullptr) {
        return this->m_pStmt->getName();
    }

    return nullptr;
}



#include "pgstmt.hpp"

#include "pgrecset.hpp"

PGrecordset::PGrecordset(DBconnection *conn, const char *command, const int nParams, const DBparameterType *paramTypes)
:   DBrecordset(conn, command, nParams, paramTypes)
{
}

PGrecordset::PGrecordset(PGconnection *conn, const char *command, const int nParams, const Oid *paramTypes)
	// This is a bit hacky, but it allows us to return the formats as an array of ints, which is what the DBparameterFormat enum is.
:   DBrecordset(conn, command, nParams, reinterpret_cast<const DBparameterType *>(paramTypes))
{
}

PGrecordset::~PGrecordset(void)
{
}

void PGrecordset::createStatement(const char *command, const int nParams, const DBparameterType *paramTypes)
{
    if (this->m_pStmt != nullptr) {
        delete this->m_pStmt;
    }

    this->m_pStmt = new PGstatement(command, nParams, paramTypes);
}



#include <iostream>

#include "pgstmt.hpp"

#include "pgrecset.hpp"

PGrecordset::PGrecordset(DBconnection *conn, const char *command, const int nParams, const DBparameterType *paramTypes, const char *stmtName)
:   DBrecordset(conn, command, nParams, paramTypes, stmtName)
{
    // It's important to call it on this level, not in the base class...
    this->init(command, nParams, paramTypes, stmtName);
}

PGrecordset::PGrecordset(PGconnection *conn, const char *command, const int nParams, const Oid *paramTypes, const char *stmtName)
	// This is a bit hacky, but it allows us to return the formats as an array of ints, which is what the DBparameterFormat enum is.
:   DBrecordset(conn, command, nParams, static_cast<const DBparameterType *>(paramTypes), stmtName)
{
    // It's important to call it on this level, not in the base class...
    this->init(command, nParams, static_cast<const DBparameterType *>(paramTypes), stmtName);
}

PGrecordset::~PGrecordset(void)
{
}

void PGrecordset::createStatement(const char *command, const int nParams, const DBparameterType *paramTypes, const char *stmtName)
{
    if (this->m_pStmt != nullptr) {
#if defined(_DEBUG)
        std::clog << "Deleting old statement " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif

        delete this->m_pStmt;
    }

    this->m_pStmt = new PGstatement(this->m_pConn, command, nParams, paramTypes);

    if (this->m_pStmt != nullptr) {
        this->m_pStmt->setName(stmtName);
    }
}

bool PGrecordset::first(void)
{
    bool ok = DBrecordset::first();

    return ok;
}

bool PGrecordset::last(void)
{
    bool ok = DBrecordset::last();

    return ok;
}

bool PGrecordset::next(void)
{
    bool ok = DBrecordset::next();

    return ok;
}

bool PGrecordset::prev(void)
{
    bool ok = DBrecordset::prev();

    return ok;
}

const DBparameterType *PGrecordset::getFieldTypes(void) const
{
    if (this->m_pStmt == nullptr) {
        std::cerr << "Need statement in getFieldName() for recordset: " << this << std::endl;

        return nullptr;
    }

    return this->m_pStmt->getFieldTypes();
}

std::string PGrecordset::getFieldName(const int field) const
{
    if (this->m_pStmt == nullptr) {
        std::cerr << "Need statement in getFieldName() for recordset: " << this << std::endl;

        return nullptr;
    }

    return this->m_pStmt->getFieldName(field);
}

int PGrecordset::getFieldNumber(const std::string fieldName) const
{
    if (this->m_pStmt == nullptr) {
        std::cerr << "Need statement in getFieldNumber() for recordset: " << this << std::endl;

        return 0;
    }

    return this->m_pStmt->getFieldNumber(fieldName);
}


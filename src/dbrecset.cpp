
#include <iostream>

#include <cstring>

#include "dbconn.hpp"

#include "dbrecset.hpp"

DBrecordset::DBrecordset([[maybe_unused]] DBconnection *conn,
                            [[maybe_unused]] const char *command,
                            [[maybe_unused]] const int nParams,
                            [[maybe_unused]] const DBparameterType *paramTypes,
                            [[maybe_unused]] const char *stmtName)
:	m_pConn(conn),
    m_pStmt(nullptr),
    m_pParam(nullptr),
    m_pAnswer(nullptr),
    nFields(0),
    nRows(0),
    currentRow(0),
    m_pTupel(nullptr)
{
    // It's important to call it on the upper, not in the base class...
    // this->init(command, nParams, paramTypes);
}

DBrecordset::~DBrecordset(void)
{
    if (this->m_pTupel != nullptr) {
#if defined(_DEBUG)
        std::clog << "Deleting tupel for recordset: " << this->m_pTupel << std::endl;
#endif

        delete this->m_pTupel;
    }

    if (this->m_pParam != nullptr) {
#if defined(_DEBUG)
        std::clog << "Deleting parameter for recordset: " << this->m_pParam << std::endl;
#endif

        delete this->m_pParam;
    }

    if (this->m_pStmt != nullptr) {
#if defined(_DEBUG)
        std::clog << "Deleting statement " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif

        delete this->m_pStmt;
    }
}

void DBrecordset::init(const char *command, const int nParams, const DBparameterType *paramTypes, const char *stmtName)
{
    this->createStatement(command, nParams, paramTypes, stmtName);

#if defined(_DEBUG)
    std::clog << "Created statement " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif

    this->prepare();
}

void DBrecordset::createStatement(const char *command, const int nParams, const DBparameterType *paramTypes, const char *stmtName)
{
    if (this->m_pStmt != nullptr) {
#if defined(_DEBUG)
        std::clog << "Deleting old statement " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif

        delete this->m_pStmt;
    }

#if defined(_DEBUG)
    std::clog << "Warning! Called createStatement() in base recordset: " << this << std::endl;
#endif

    this->m_pStmt = new DBstatement(this->m_pConn, command, nParams, paramTypes);

    if (this->m_pStmt != nullptr) {
        this->m_pStmt->setName(stmtName);
    }
}

void DBrecordset::prepare(void)
{
    if (! this->check()) {
		return;
	}

    if (this->m_pStmt != nullptr) {
        this->m_pStmt->prepare();

#if defined(_DEBUG)
        std::clog << "Prepared statement " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif
    }
}

bool DBrecordset::check(void)
{
    if (this->m_pConn != nullptr) {
        return this->m_pConn->check();
    } else {
        return false;
    }
}

const DBanswer *DBrecordset::exec(const char *errmsg, const DBparameterFormat resultFormat)
{
    const DBanswer *ret = nullptr;

    if (! this->check()) {
		return nullptr;
	}

    if (this->m_pStmt != nullptr) {
        ret = this->m_pStmt->exec(errmsg, resultFormat);

#if defined(_DEBUG)
        std::clog << "Called exec() for statement  " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif
    }

    return ret;
}

const DBanswer *DBrecordset::exec(const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
    const DBanswer *ret = nullptr;

    if (! this->check()) {
		return nullptr;
	}

    if (this->m_pStmt != nullptr) {
        ret = this->m_pStmt->exec(param, errmsg, resultFormat);

#if defined(_DEBUG)
        std::clog << "Called exec() for statement  " << this->m_pStmt->getName() << " for recordset: " << this << std::endl;
#endif
    }

    return ret;
}

std::string DBrecordset::getanswer(const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(errmsg, resultFormat);

	const std::string answer = this->m_pConn->getanswer(answ, resultFormat);

	return answer;
}

std::string DBrecordset::getanswer(const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *answ = this->exec(param, errmsg, resultFormat);

	const std::string answer = this->m_pConn->getanswer(answ, resultFormat);

	return answer;
}

const char *DBrecordset::getStmtName(void) const
{
    if (this->m_pStmt != nullptr) {
        return this->m_pStmt->getName();
    }

    return nullptr;
}

const char *DBrecordset::getCommand(void) const
{
    if (this->m_pStmt != nullptr) {
        return this->m_pStmt->getCommand();
    }

    return nullptr;
}

const DBparameterType *DBrecordset::getParamTypes(void) const
{
    if (this->m_pStmt != nullptr) {
        return this->m_pStmt->getParamTypes();
    }

    return nullptr;
}

DBstatement *DBrecordset::getStatement(void) const
{
    return this->m_pStmt;
}

DBparameter *DBrecordset::getParameter(void) const
{
    return this->m_pParam;
}

DBtupel *DBrecordset::getTupel(void) const
{
    return this->m_pTupel;
}

bool DBrecordset::first(void)
{
    this->currentRow = 0;

    return this->hasData();
}

bool DBrecordset::last(void)
{
    this->currentRow = this->nRows - 1;

    return this->hasData();
}

bool DBrecordset::next(void)
{
    if ((this->m_pTupel != nullptr) && (this->currentRow < this->nRows - 1)) {
        ++this->currentRow;
        return true;
    }

    return false;
}

bool DBrecordset::prev(void)
{
    if ((this->m_pTupel != nullptr) && (this->currentRow > 0)) {
        --this->currentRow;
        return true;
    }

    return false;
}

bool DBrecordset::isFirst(void) const
{
    return this->currentRow == 0;
}

bool DBrecordset::isLast(void) const
{
    return this->currentRow == this->nRows - 1;
}

bool DBrecordset::hasData(void) const
{
    return ((this->m_pTupel != nullptr) && (this->m_pTupel->count() > 0));
}

bool DBrecordset::isEmpty(void) const
{
    return this->nRows == 0;
}

void DBrecordset::reset(void)
{
    this->first();
}

int DBrecordset::getCurrentRow(void) const
{
    return (this->currentRow + 1);  // Adjust for 1-based indexing
}

int DBrecordset::getNRows(void) const
{
    return this->nRows;
}

int DBrecordset::getNFields(void) const
{
    return this->nFields;
}

const char *DBrecordset::getValue(const int field, const char *defaultValue) const
{
    int i = field - 1;  // Adjust for 1-based indexing

    if (i < 0 || i >= this->nFields) {
        return defaultValue;
    }

    return this->m_pTupel->values()[i];
}

const char *DBrecordset::getValue(const char *fieldName, const char *defaultValue) const
{
/*
    for (int i = 0; i < this->nFields; ++i) {
        if (strcmp(this->getFieldName(i), fieldName) == 0) {
            return this->getValue(i, defaultValue);
        }
    }
*/

    int field = this->getFieldNumber(fieldName);

    if (field > 0) {
        return this->getValue(field, defaultValue);
    } else {
        return defaultValue;
    }
}


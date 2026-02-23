
#include "dbconn.hpp"

#include "dbstmt.hpp"

DBstatement::DBstatement(const std::string &command, const int nParams, const DBparameterType *paramTypes)
:	m_name(this->getautoname()),
    m_command(command),
    m_nParams(nParams),
    m_paramTypes(paramTypes),
	m_nFields(0),
	m_fieldTypes(nullptr),
	m_fieldNames(nullptr)
{
}

DBstatement::~DBstatement(void)
{
	this->resize(0);
}

void DBstatement::setName(const char *name)
{
	if (name != nullptr) {
		m_name = name;
	}
}

std::string DBstatement::getautoname(void) const
{
    return "stmt_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
}

const char *DBstatement::getName(void) const
{
	return m_name.c_str();
}

const char *DBstatement::getCommand(void) const
{
	return m_command.c_str();
}

int DBstatement::getNParams(void) const
{
	return m_nParams;
}

const DBparameterType *DBstatement::getParamTypes(void) const
{
	return m_paramTypes;
}

void DBstatement::resize(int nFields)
{
	if (this->m_fieldTypes != nullptr) {
		delete[] this->m_fieldTypes;
	}

	if (this->m_fieldNames != nullptr) {
		delete[] this->m_fieldNames;
	}

	this->m_nFields = nFields;

	if (nFields > 0) {
		this->m_fieldTypes = new DBparameterType[nFields];

		this->m_fieldNames = new std::string[nFields];
	}
}

void DBstatement::setNFields(int nFields)
{
	this->resize(nFields);
}

int DBstatement::getNFields(void) const
{
	return this->m_nFields;
}

const DBparameterType *DBstatement::getFieldTypes(void) const
{
	return this->m_fieldTypes;
}

std::string DBstatement::getFieldName(int field) const
{
	int i = field - 1;  // Adjust for 1-based indexing

	return this->m_fieldNames[i];
}


int DBstatement::getFieldNumber([[maybe_unused]] const std::string fieldName) const
{
	// Default implementation does nothing, but derived classes can override this to get the information for the statement on the connection.
	int field = 0;

	return (field + 1);  // Adjust for 1-based indexing
}

void DBstatement::prepare(DBconnection *conn)
{
	// Default implementation does nothing, but derived classes can override this to prepare the statement on the connection.
	this->calcFieldInfos(conn);
}

void DBstatement::calcFieldInfos([[maybe_unused]] DBconnection *conn)
{
	// Default implementation does nothing, but derived classes can override this to calculate infos after preparation of the statement on the connection.
}

const DBanswer *DBstatement::exec(DBconnection *conn, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *ret = nullptr;

	if (conn != nullptr) {
		ret = conn->exec(this, errmsg, resultFormat);
	}

	return ret;
}

const DBanswer *DBstatement::exec(DBconnection *conn, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
	const DBanswer *ret = nullptr;

	if (conn != nullptr) {
		ret = conn->exec(this, param, errmsg, resultFormat);
	}

	return ret;
}



#include "dbstmt.hpp"

DBstatement::DBstatement(const std::string &command, const int nParams, const DBparameterType *paramTypes)
:	m_name(this->getautoname()),
    m_command(command),
    m_nParams(nParams),
    m_paramTypes(paramTypes)
{
}

DBstatement::~DBstatement(void)
{
}

void DBstatement::setName(const char *name)
{
	if (name != nullptr) {
		m_name = name;
	}
}

const std::string DBstatement::getautoname(void) const
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

const int DBstatement::getNParams(void) const
{
	return m_nParams;
}

const DBparameterType *DBstatement::getParamTypes(void) const
{
	return m_paramTypes;
}


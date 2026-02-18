
#include "pgstmt.hpp"

const std::string PGstatement::m_autoname_prefix = "stmt_";
int PGstatement::m_autoname_index = 0;

PGstatement::PGstatement(const std::string &command, const int nParams, const DBparameterType *paramTypes)
:	DBstatement(command, nParams, paramTypes)
{
}

PGstatement::~PGstatement(void)
{
}

const std::string PGstatement::getautoname(void) const
{
    return PGstatement::m_autoname_prefix + std::to_string(PGstatement::m_autoname_index++);
}


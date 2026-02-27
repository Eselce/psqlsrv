
#include <iostream>

#include "pgconn.hpp"

#include "pgstmt.hpp"

const std::string PGstatement::m_autoname_prefix = "stmt_";
int PGstatement::m_autoname_index = 0;

PGstatement::PGstatement(DBconnection *conn, const std::string &command, const int nParams, const DBparameterType *paramTypes)
:	DBstatement(conn, command, nParams, paramTypes)
{
}

PGstatement::~PGstatement(void)
{
    if (m_pRes != nullptr) {
        PQclear(m_pRes);

#if defined(_DEBUG)
        std::clog << "Cleared prepared statement " << this->getName() << ": " << this << std::endl;
		m_pRes = nullptr;
#endif
    }
}

std::string PGstatement::getautoname(void) const
{
    return PGstatement::m_autoname_prefix + std::to_string(PGstatement::m_autoname_index++);
}

int PGstatement::getFieldNumber([[maybe_unused]] const std::string fieldName) const
{
	int field = 0;  // TODO

/*
    if ((this->m_pConn == nullptr) || ! this->m_pConn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("getFieldNumber(): Need an open connection to get infos for the statement");
    } else {
#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            std::clog << "Getting field number: " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

        m_pRes = PQdescribePrepared(this->m_pConn->getPGconn(), this->getName());

#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Describe prepared statement " << this.getName() << " " << this << ": status = " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
        }
#endif

    field = PQfnumber(m_pRes, fieldName.c_str());

#if defined(_DEBUG)
        PQclear(m_pRes);

        m_pRes = nullptr;
#endif
*/

	return (field + 1);  // Adjust for 1-based indexing
}

void PGstatement::prepare(void)
{
    if ((this->m_pConn == nullptr) || ! this->m_pConn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("prepare(): Need an open connection to prepare the statement");
    } else {
#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            std::clog << "Preparing statement " << this->getName() << ": " << this << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

        PGconnection *pConn = dynamic_cast<PGconnection *>(this->m_pConn);

#if defined(_DEBUG)
        // Same action, but with intermediate values for debugging...
        PGconn *pgConn = pConn->getPGconn();
        const char *stmtName = this->getName();
        const char *query = this->getCommand();
        int nParams = this->getNParams();
        const Oid *paramTypes = this->getParamTypes();

        m_pRes = PQprepare(pgConn, stmtName, query, nParams, paramTypes);
#else
        m_pRes = PQprepare(pConn->getPGconn(), this->getName(), this->getCommand(), this->getNParams(), this->getParamTypes());
#endif

#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Prepared statement " << this->getName() << ": " << this << " status = " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
        }

        this->m_pConn->printerror();

        PQclear(m_pRes);

        m_pRes = nullptr;
#endif

        DBstatement::prepare();  // Base class, triggers calcFieldInfos()...
    }
}

void PGstatement::calcFieldInfos(void)
{
    if ((this->m_pConn == nullptr) || ! this->m_pConn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("calcFieldInfos(): Need an open connection to prepare the statement");
    } else {
#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            std::clog << "Calculating field data: " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

        PGconnection *pConn = dynamic_cast<PGconnection *>(this->m_pConn);

        m_pRes = PQdescribePrepared(pConn->getPGconn(), this->getName());

#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Describe prepared statement " << this->getName() << ": " << this << " status = " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
        }
#endif

#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            const int nParams = PQnparams(m_pRes);
            std::string params = "[" + std::to_string(nParams) + "] {";

            for (int i = 0, param = i + 1; i < nParams; ++i, ++param) {
                const Oid pType = PQparamtype(m_pRes, i);
	            const DBparameterType parameterType = static_cast<DBparameterType>(pType);

                params += "$" + std::to_string(param) + "(" + std::to_string(parameterType) + ")";

		    	if (i < (nParams - 1)) {
			        params += ",";
			    }
            }
    		params += "}";

            std::clog << "Info for statement (params): " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]: " << params << std::endl;
        }
#endif

#if defined(_DEBUG)
        const int nTuples = PQntuples(m_pRes);
#endif
        const int nFields = PQnfields(m_pRes);

        this->setNFields(nFields);

        for (int i = 0; i < nFields; ++i) {
            const char *fName = PQfname(m_pRes, i);
            const Oid fType = PQftype(m_pRes, i);
            std::string fieldName = fName;
	        const DBparameterType fieldType = static_cast<DBparameterType>(fType);

            this->m_fieldNames[i] = fieldName;
            this->m_fieldTypes[i] = fieldType;
        }

#if defined(_DEBUG)
        if (this->m_pConn->getVerbose()) {
            std::clog << "Info for statement (fields): " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]: "
                        << nTuples << " x " << nFields << std::endl;
        }
#endif

        PQclear(m_pRes);

#if defined(_DEBUG)
        m_pRes = nullptr;
#endif
    }
}


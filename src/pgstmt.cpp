
#include <iostream>

#include "pgconn.hpp"

#include "pgstmt.hpp"

const std::string PGstatement::m_autoname_prefix = "stmt_";
int PGstatement::m_autoname_index = 0;

PGstatement::PGstatement(const std::string &command, const int nParams, const DBparameterType *paramTypes)
:	DBstatement(command, nParams, paramTypes)
{
}

PGstatement::~PGstatement(void)
{
    if (m_pRes != nullptr) {
        PQclear(m_pRes);

#if defined(_DEBUG)
        std::clog << "Cleared prepared statement: " << this->getName() << std::endl;
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
    if ((conn == nullptr) || ! conn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("getFieldNumber(): Need an open connection to get infos for the statement");
    } else {
#if defined(_DEBUG)
        if (conn->getVerbose()) {
            std::clog << "Getting field number: " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

        m_pRes = PQdescribePrepared(conn->getPGconn(), this->getName());

#if defined(_DEBUG)
        if (conn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Describe prepared statement (status): " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
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

void PGstatement::prepare(DBconnection *conn)
{
    this->prepare(dynamic_cast<PGconnection *>(conn));
}

void PGstatement::prepare(PGconnection *conn)
{
    if ((conn == nullptr) || ! conn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("prepare(): Need an open connection to prepare the statement");
    } else {
#if defined(_DEBUG)
        if (conn->getVerbose()) {
            std::clog << "Preparing statement: " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

#if defined(_DEBUG)
        PGconn *pConn = conn->getPGconn();
        const char *stmtName = this->getName();
        const char *query = this->getCommand();
        int nParams = this->getNParams();
        const Oid *paramTypes = this->getParamTypes();

        m_pRes = PQprepare(pConn, stmtName, query, nParams, paramTypes);
#else
        m_pRes = PQprepare(conn->getPGconn(), this->getName(), this->getCommand(), this->getNParams(), this->getParamTypes());
#endif

#if defined(_DEBUG)
        if (conn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Prepared statement (status): " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
        }

        PQclear(m_pRes);

        m_pRes = nullptr;
#endif

        DBstatement::prepare(conn);  // Base class, triggers calcFieldInfos()...
    }
}

void PGstatement::calcFieldInfos(DBconnection *conn)
{
    this->calcFieldInfos(dynamic_cast<PGconnection *>(conn));
}

void PGstatement::calcFieldInfos(PGconnection *conn)
{
    if ((conn == nullptr) || ! conn->check()) {
        m_pRes = nullptr;

        throw std::invalid_argument("calcFieldInfos(): Need an open connection to prepare the statement");
    } else {
#if defined(_DEBUG)
        if (conn->getVerbose()) {
            std::clog << "Calculating field data: " << this->getName() << " = " << this->getCommand()
                        << "[" << this->getNParams() << "]" << std::endl;
        }
#endif

        m_pRes = PQdescribePrepared(conn->getPGconn(), this->getName());

#if defined(_DEBUG)
        if (conn->getVerbose()) {
            ExecStatusType status = PQresultStatus(m_pRes);

            std::clog << "Describe prepared statement (status): " << status << " [" << ExecStatusTypeName[status] << "]" << std::endl;
        }
#endif

#if defined(_DEBUG)
        if (conn->getVerbose()) {
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
        if (conn->getVerbose()) {
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

const DBanswer *PGstatement::exec(DBconnection *conn, const char *errmsg, const DBparameterFormat resultFormat)
{
    return this->exec(dynamic_cast<PGconnection *>(conn), errmsg, resultFormat);
}

const DBanswer *PGstatement::exec(PGconnection *conn, const char *errmsg, const DBparameterFormat resultFormat)
{
    return DBstatement::exec(conn, errmsg, resultFormat);
}

const DBanswer *PGstatement::exec(DBconnection *conn, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
    return this->exec(dynamic_cast<PGconnection *>(conn), param, errmsg, resultFormat);
}

const DBanswer *PGstatement::exec(PGconnection *conn, const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat)
{
    return DBstatement::exec(conn, param, errmsg, resultFormat);
}

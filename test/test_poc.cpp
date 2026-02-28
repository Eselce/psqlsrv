
#include <endian.h>

#include "pg_type.h"

#include "pgparam.hpp"

#include "pgstmt.hpp"

#include "pganswer.hpp"

#include "test_poc.hpp"

test_poc::test_poc(void)
{
}

test_poc::~test_poc(void)
{
}

bool test_poc::classrun(PGconnection &pg, const char *command, const int arg) const {
    const char *stmtname = "mystat";
    PGparameter param;

    param.bind(arg);

    DBrecordset *recset = pg.query(command, param, stmtname);

    std::cout << "ANSWER: " << recset->getanswer(param, cmdErrorMsg, FORMAT_TEXT) << std::endl << std::endl;

    delete recset;

    // Now in destructor of statement!
    //this->freestmt(pg, stmtname);

    return true;
}

bool test_poc::autorun(PGconnection &pg, const char *command, const int arg) const {
    const char *stmtname = "mystat";
    const Oid paramTypes[] = { INT4OID };
    const int nParams = (sizeof(paramTypes) / sizeof(Oid));
    PGstatement stmt(&pg, command, nParams, paramTypes);
    PGparameter param;

    param.bind(arg);

    stmt.setName(stmtname);

    stmt.prepare();

    const DBanswer *answ = stmt.exec(param, cmdErrorMsg, FORMAT_TEXT);

    if (answ != nullptr) {
        std::cout << "ANSWER: " << answ->getanswer(FORMAT_TEXT) << std::endl << std::endl;
    }

    // Now in destructor of statement!
    //this->freestmt(pg, stmtname);

    return true;
}

bool test_poc::manualrun(PGconnection &pg, const char *command, const int arg) const {
    const char *stmtname = "mystat";
    const Oid paramTypes[] = { INT4OID };
    const int nParams = (sizeof(paramTypes) / sizeof(Oid));
    const int paramData[] = { htobe32(arg) };  // PQ expects big endian values in binary data
    const int *paramPointers[] = { paramData };
    char **paramValues = reinterpret_cast<char **>(const_cast<int **>(paramPointers));
    const int paramLengths[] = { sizeof(arg) };
    const int paramFormats[] = { FORMAT_BINARY };

    PGresult *res = PQprepare(pg.getPGconn(), stmtname, command, nParams, paramTypes);
    ExecStatusType status = PQresultStatus(res);

    std::clog << "Prepared statement " << stmtname << ": status = " << status
#if defined(_DEBUG)
                << " [" << ExecStatusTypeName[status] << "]"
#endif
                << std::endl;

    if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
        pg.printerror();

        std::clog << "PQprepare returned error status: " << status
#if defined(_DEBUG)
                << " [" << ExecStatusTypeName[status] << "]"
#endif
                << std::endl;
    }

    PQclear(res);

    std::clog << "Cleared result: " << pg.getPGconn() << std::endl;

    PGresult *exeres = PQexecPrepared(pg.getPGconn(), stmtname, nParams, paramValues, paramLengths, paramFormats, FORMAT_TEXT);
    status = PQresultStatus(exeres);

    std::clog << "Execute statement " << stmtname << ": status = " << status
#if defined(_DEBUG)
                << " [" << ExecStatusTypeName[status] << "]"
#endif
                << std::endl;

    if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
        pg.printerror();

        PQclear(exeres);

        std::clog << "PQexecPrepared returned error status = " << status
#if defined(_DEBUG)
                << " [" << ExecStatusTypeName[status] << "]"
#endif
                << std::endl;
        std::clog << "Cleared result: " << pg.getPGconn() << std::endl;

        return false;
    }

    const PGanswer answ(exeres);

    std::cout << "ANSWER: " << answ.getanswer(FORMAT_TEXT) << std::endl << std::endl;

    this->freestmt(pg, stmtname);

    return true;
}

void test_poc::freestmt(PGconnection &pg, const char *stmtname) const {
    // Freeing statement quick and dirty, there may be no API function for it...
    std::string deallocate = "DEALLOCATE PREPARE ";

    deallocate += stmtname;

    pg.getanswer(deallocate.c_str()); 
}

bool test_poc::run(void)
{
    const char    *command = "SELECT * FROM test2 WHERE zahl > $1;";
    const int      arg = 50;
    const char    *conninfo = "";
    PGconnection   pg;
    bool           ret = false;

    pg.connect(conninfo, true, PQERRORS_VERBOSE);

   	if (pg.checkconnect()) {
        PQsetErrorVerbosity(pg.getPGconn(), PQERRORS_VERBOSE);

		std::cout << "ANSWER: " << pg.getanswer("SELECT version();") << std::endl;
		std::cout << "ANSWER: " << pg.getanswer("SET search_path TO loges;") << std::endl;

        ret = manualrun(pg, command, arg);

        ret = autorun(pg, command, arg);

        ret = classrun(pg, command, arg);
    }

    return ret;
}


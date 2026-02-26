
#include "pg_type.h"

#include "pgconnex.hpp"

PGconnectionEx::PGconnectionEx(void)
:	PGconnection()
{
}

PGconnectionEx::PGconnectionEx(const std::string &connectstr, const bool blocking, const int verboselevel)
:	PGconnection(connectstr, blocking, verboselevel)
{
}

PGconnectionEx::PGconnectionEx(const char *conninfo, const bool blocking, const int verboselevel)
:	PGconnection(conninfo, blocking, verboselevel)
{
}

// const int expand_dbname = 0;  // every entry is separate (0) or dbname contains parameters (1)
PGconnectionEx::PGconnectionEx(const char **keys, const char **vals, const bool blocking, const int verboselevel, const int expand_dbname)
:	PGconnection(keys, vals, blocking, verboselevel, expand_dbname)
{
}

PGconnectionEx::~PGconnectionEx(void)
{
}


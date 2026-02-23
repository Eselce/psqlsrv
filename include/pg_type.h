
#pragma once

#include "postgresql/libpq-fe.h"

#include "postgresql/pg_config.h"

class PGconnection;  // forward declaration to avoid circular dependency

// Substitution for server side pg_type.h
// until it's clear what the fuzz is about!

#if !defined(OIDOID)

#define BOOLOID		    16
#define BYTEAOID	    17
#define INT2OID		    21
#define INT4OID		    23
#define TEXTOID		    25
#define OIDOID	    	26

#define CHAROID         18
#define NAMEOID         19
#define INT8OID		    20
#define INT2VECTOROID	22
#define REGPROCOID	    24
#define TIDOID		    27
#define XIDOID		    28
#define CIDOID		    29
#define OIDVECTOROID	30
#define PGTYPEOID	    71
#define PGATTRIBUTEOID	75
#define PGPROCOID	    81
#define PGCLASSOID      83

// These are not in pg_type.h but are needed for the OID values
// Could be in pg_type.h but that would require including it in dbparam.hpp which is not ideal
// Compiled from pg_type.h in PostgreSQL 15.5 (although we have PostgreSQL 14 installed, these OIDs are the same in both versions)
// Copilot is not able to find these in pg_type.h but they are needed for the OID values, so they are added here as a workaround
// Copilot is suggesting a lot  of other OIDs that are not needed, so they are not included here
// Copilot is suggesting a lot of strange text, but the pg_class OID is actually 83, not 78, so that is corrected here as well
//#define PGCLASSOID    78

#define JSONOID		    114
#define XMLOID		    142

#define VARCHAROID	    1043
#define BPCHAROID	    1042
#define FLOAT4OID	    700
#define FLOATOID        700
#define FLOAT8OID	    701
#define DOUBLEOID       701
#define TIMESTAMPTZOID	1184
#define TIMESTAMPOID	1114
#define TIMETZOID	    1266
#define TIMEOID		    1083
#define DATEOID		    1082

#define DECIMALOID	    1700
#define NUMERICOID	    1700
#define JSONBOID	    3802
#define UUIDOID		    2950

#endif

#if defined(_DEBUG)

// Names of some constants (primitive implementation)
// TODO Create a more clever implementation. Debug only.

/* Application-visible enum types */

/*
 * Although it is okay to add to these lists, values which become unused
 * should never be removed, nor should constants be redefined - that would
 * break compatibility with existing code.
 */

[[maybe_unused]] static const char *ConnStatusTypeName[] =
{
	"CONNECTION_OK",
	"CONNECTION_BAD",
	/* Non-blocking mode only below here */

	/*
	 * The existence of these should never be relied upon - they should only
	 * be used for user feedback or similar purposes.
	 */
	"CONNECTION_STARTED",		/* Waiting for connection to be made.  */
	"CONNECTION_MADE",			/* Connection OK; waiting to send.     */
	"CONNECTION_AWAITING_RESPONSE",	/* Waiting for a response from the
									 * postmaster.        */
	"CONNECTION_AUTH_OK",		/* Received authentication; waiting for
								 * backend startup. */
	"CONNECTION_SETENV",		/* This state is no longer used. */
	"CONNECTION_SSL_STARTUP",	/* Negotiating SSL. */
	"CONNECTION_NEEDED",		/* Internal state: connect() needed */
	"CONNECTION_CHECK_WRITABLE",/* Checking if session is read-write. */
	"CONNECTION_CONSUME",		/* Consuming any extra messages. */
	"CONNECTION_GSS_STARTUP",	/* Negotiating GSSAPI. */
	"CONNECTION_CHECK_TARGET",	/* Checking target server properties. */
	"CONNECTION_CHECK_STANDBY"	/* Checking if server is in standby mode. */
};

[[maybe_unused]] static const char *PostgresPollingStatusTypeName[] =
{
	"PGRES_POLLING_FAILED",
	"PGRES_POLLING_READING",	/* These two indicate that one may	  */
	"PGRES_POLLING_WRITING",	/* use select before polling again.   */
	"PGRES_POLLING_OK",
	"PGRES_POLLING_ACTIVE"		/* unused; keep for awhile for backwards
								 * compatibility */
};

[[maybe_unused]] static const char *ExecStatusTypeName[] =
{
	"PGRES_EMPTY_QUERY",		/* empty query string was executed */
	"PGRES_COMMAND_OK",			/* a query command that doesn't return
								 * anything was executed properly by the
								 * backend */
	"PGRES_TUPLES_OK",			/* a query command that returns tuples was
								 * executed properly by the backend, PGresult
								 * contains the result tuples */
	"PGRES_COPY_OUT",			/* Copy Out data transfer in progress */
	"PGRES_COPY_IN",			/* Copy In data transfer in progress */
	"PGRES_BAD_RESPONSE",		/* an unexpected response was recv'd from the
								 * backend */
	"PGRES_NONFATAL_ERROR",		/* notice or warning message */
	"PGRES_FATAL_ERROR",		/* query failed */
	"PGRES_COPY_BOTH",			/* Copy In/Out data transfer in progress */
	"PGRES_SINGLE_TUPLE",		/* single tuple from larger resultset */
	"PGRES_PIPELINE_SYNC",		/* pipeline synchronization point */
	"PGRES_PIPELINE_ABORTED"	/* Command didn't run because of an abort
								 * earlier in a pipeline */
};

[[maybe_unused]] static const char *PGTransactionStatusTypeName[] =
{
	"PQTRANS_IDLE",				/* connection idle */
	"PQTRANS_ACTIVE",			/* command in progress */
	"PQTRANS_INTRANS",			/* idle, within transaction block */
	"PQTRANS_INERROR",			/* idle, within failed transaction */
	"PQTRANS_UNKNOWN"				/* cannot determine status */
};

[[maybe_unused]] static const char *PGVerbosityName[] =
{
	"PQERRORS_TERSE",			/* single-line error messages */
	"PQERRORS_DEFAULT",			/* recommended style */
	"PQERRORS_VERBOSE",			/* all the facts, ma'am */
	"PQERRORS_SQLSTATE"			/* only error severity and SQLSTATE code */
};

[[maybe_unused]] static const char *PGContextVisibilityName[] =
{
	"PQSHOW_CONTEXT_NEVER",		/* never show CONTEXT field */
	"PQSHOW_CONTEXT_ERRORS",	/* show CONTEXT for errors only (default) */
	"PQSHOW_CONTEXT_ALWAYS"		/* always show CONTEXT field */
};

/*
 * PGPing - The ordering of this enum should not be altered because the
 * values are exposed externally via pg_isready.
 */

[[maybe_unused]] static const char *PGPingName[] =
{
	"PQPING_OK",				/* server is accepting connections */
	"PQPING_REJECT",			/* server is alive but rejecting connections */
	"PQPING_NO_RESPONSE",		/* could not establish connection */
	"PQPING_NO_ATTEMPT"			/* connection not attempted (bad params) */
};

/*
 * PGpipelineStatus - Current status of pipeline mode
 */
[[maybe_unused]] static const char *PGpipelineStatusName[] =
{
	"PQ_PIPELINE_OFF",
	"PQ_PIPELINE_ON",
	"PQ_PIPELINE_ABORTED"
};

#endif


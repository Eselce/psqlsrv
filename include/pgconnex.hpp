
#pragma once

#include "pg_type.h"

#include "pgconn.hpp"

class PGconnectionEx : public PGconnection
{
public:
	PGconnectionEx(void);

	PGconnectionEx(const std::string &connectstr, const bool blocking = false, const int verboselevel = PQERRORS_VERBOSE);

	PGconnectionEx(const char *conninfo, const bool blocking = false, const int verboselevel = PQERRORS_VERBOSE);

	PGconnectionEx(const char **keys,
					const char **vals,
					const bool blocking = false,
					const int verboselevel = PQERRORS_VERBOSE,
					const int expand_dbname = 0);

	virtual ~PGconnectionEx(void) override;

public:
	inline char *database(void) {
		return PQdb(this->getPGconn());
	}

	inline char *user(void) {
		return PQuser(this->getPGconn());
	}

	inline char *host(void) {
		return PQhost(this->getPGconn());
	}

	inline char *hostaddr(void) {
		return PQhostaddr(this->getPGconn());
	}

	inline char *portstring(void) {
		return PQport(this->getPGconn());
	}

	inline unsigned int port(void) {
		return std::stoi(this->portstring());
	}

	inline char *options(void) {
		return PQoptions(this->getPGconn());
	}

	inline ConnStatusType status(void) {
		return PQstatus(this->getPGconn());
	}

	inline PGTransactionStatusType transactionStatus(void) {
		return PQtransactionStatus(this->getPGconn());
	}

	// paramName:
	// application_name 				scram_iterations
	// client_encoding 					search_path
	// DateStyle 						server_encoding
	// default_transaction_read_only 	server_version
	// in_hot_standby 					session_authorization
	// integer_datetimes 				standard_conforming_strings
	// IntervalStyle 					TimeZone
	// is_superuser 	 
	inline const char *parameterStatus(const char *paramName) {
		return PQparameterStatus(this->getPGconn(), paramName);
	}

/***
	inline int fullProtocolVersion(void) {
		return PQfullProtocolVersion(this->getPGconn());
	}
***/

	inline int protocolVersion(void) {
		return PQprotocolVersion(this->getPGconn());
	}

	inline int serverVersion(void) {
		return PQserverVersion(this->getPGconn());
	}

	inline char *errorMessage(void) {
		return PQerrorMessage(this->getPGconn());
	}

	inline int socket(void) {
		return PQsocket(this->getPGconn());
	}

	inline int backendPID(void) {
		return PQbackendPID(this->getPGconn());
	}

	inline int connectionNeedsPassword(void) {
		int needsPassword = PQconnectionNeedsPassword(this->getPGconn());

		return (needsPassword == 1);
	}

	inline int connectionUsedPassword(void) {
		int usedPassword = PQconnectionUsedPassword(this->getPGconn());

		return (usedPassword == 1);
	}

/***
	inline int connectionUsedGSSAPI(void) {
		int usedGSSAPI = PQconnectionUsedGSSAPI(this->getPGconn());

		return (usedGSSAPI == 1);
	}
***/

	inline int gssEncInUse(void) {
		int encInUse = PQgssEncInUse(this->getPGconn());

		return (encInUse == 1);
	}

	inline int sslInUse(void) {
		int inUse = PQsslInUse(this->getPGconn());

		return (inUse == 1);
	}

	// attribute_name:
	// library
	// protocol
	// key_bits
	// cipher
	// compression
	// alpn
	inline const char *sslAttribute(const char *attribute_name) {
		return PQsslAttribute(this->getPGconn(), attribute_name);
	}

	inline const char * const * sslAttributeNames(void) {
		return PQsslAttributeNames(this->getPGconn());
	}

	inline void *sslStruct(const char *struct_name) {
		return PQsslStruct(this->getPGconn(), struct_name);
	}

	inline void *getssl(void) {
		return PQgetssl(this->getPGconn());
	}

	inline PGVerbosity setErrorVerbosity(PGVerbosity verbosity) {
		return PQsetErrorVerbosity(this->getPGconn(), verbosity);
	}

private:
	inline char *password(void) {
		return PQpass(this->getPGconn());
	}

	inline char *tty(void) {
		return PQtty(this->getPGconn());
	}

protected:
};


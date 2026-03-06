
#include "pg_type.h"

#include "pgconn.hpp"

#include "pgparam.hpp"

#include "test_param.hpp"

test_param::test_param(void)
{
}

test_param::~test_param(void)
{
}

bool test_param::classrun([[maybe_unused]] PGconnection &pg, [[maybe_unused]] const char *command, const int arg) const {
	const char *parsestring = "50.5	50.5	50.5	50.5	50.5	test";
	PGparameter param(static_cast<int>(arg),
						static_cast<short int>(arg),
						static_cast<long int>(arg),
						static_cast<float>(arg),
						static_cast<double>(arg),
						"arg");

	param.parse(parsestring, '	');

	std::cout << "ANSWER: " << param.to_string() << std::endl << std::endl;

	return true;
}

bool test_param::run(void)
{
	const char		*command = "SELECT * FROM test2 WHERE zahl > $1;";
	const int		 arg = 50;
	const char		*conninfo = "";
	PGconnection	 pg;
	bool			 ret = false;

	pg.connect(conninfo, true, PQERRORS_VERBOSE);

   	if (pg.checkconnect()) {
		PQsetErrorVerbosity(pg.getPGconn(), PQERRORS_VERBOSE);

		std::cout << "ANSWER: " << pg.getanswer("SELECT version();") << std::endl;
		std::cout << "ANSWER: " << pg.getanswer("SET search_path TO loges;") << std::endl;

		ret = classrun(pg, command, arg);
	}

	return ret;
}


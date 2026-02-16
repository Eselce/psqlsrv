
#define _START_HTTP_SERVER_MAIN
#define _START_PG_CONNECTION_MAIN

#include "main.hpp"

int main(int argc, char **argv)
{
#if defined(_START_HTTP_SERVER_MAIN)
	bool           ok       = false;
#else
	bool           ok       = true;
#endif

#if defined(_START_PG_CONNECTION_MAIN)
	std::cout << "STARTING POSTGRESQL CONNECTION" << std::endl;

	const char    *conninfo;
	PGconnection   pg;

	if (argc > 1) {
		conninfo = argv[1];
	} else {
		conninfo = "";
	}

	pg.connect(conninfo, true);

	if (pg.checkconnect()) {
		std::cout << "CONNECTED" << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT version();") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT current_database();") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT current_user;") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT current_schema();") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SET search_path TO loges;") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT 1;") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT * FROM pg_type LIMIT 10;") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT * FROM orders LIMIT 3;") << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT zahl, klein FROM test2 WHERE \"ID\" = 2;") << std::endl;
#endif

#if defined(_START_HTTP_SERVER_MAIN)
		std::cout << "STARTING HTTP SERVER" << std::endl;

		HTTPserver srv;

		srv.setupStatic();

#if defined(_START_PG_CONNECTION_MAIN)
		srv.setDBConnection(&pg);
#endif

		ok = srv.start();
#endif

#if defined(_START_PG_CONNECTION_MAIN)
		pg.disconnect();
	} else {
		std::cerr << "CONNECTION FAILED" << std::endl;
		ok = false;
	}
#endif

	return (ok ? 0 : -1);
}


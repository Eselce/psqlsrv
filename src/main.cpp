
//#define _START_HTTP_SERVER_MAIN
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

		PGparameter parama(1);
		PGparameter paramb(3);

		parama.set(2, 1);
		paramb.set("zahl", 1);
		paramb.set("klein", 2);
		paramb.set(2, 3);

		std::cout << "ANSWER: " << pg.getanswer("SELECT zahl, klein FROM test2 WHERE \"ID\" = $1;", parama) << std::endl;

		std::cout << "ANSWER: " << pg.getanswer("SELECT $1, $2 FROM test2 WHERE \"ID\" = $3;", paramb) << std::endl;

		DBrecordset *recset = pg.query("SELECT * FROM test2;");

		if (recset != nullptr) {
			std::cout << "RECORDSET CREATED" << std::endl;

			delete recset;
		} else {
			std::cerr << "FAILED TO CREATE RECORDSET" << std::endl;
		}

		recset = pg.query("SELECT * FROM test2;", "test17");

		if (recset != nullptr) {
			std::cout << "RECORDSET CREATED" << std::endl;

			delete recset;
		} else {
			std::cerr << "FAILED TO CREATE RECORDSET" << std::endl;
		}

		PGparameter paramc(1);
		paramc.set(50, 1);

		recset = pg.query("SELECT * FROM test2 WHERE zahl > $1;", paramc, "test21");

		if (recset != nullptr) {
			std::cout << "RECORDSET CREATED" << std::endl;

			delete recset;
		} else {
			std::cerr << "FAILED TO CREATE RECORDSET" << std::endl;
		}
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


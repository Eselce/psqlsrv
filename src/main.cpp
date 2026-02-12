
#include "main.hpp"

int main(int argc, char **argv)
{
	const char    *conninfo;
	PGconnection   pg;
	bool           ok       = false;

	if (argc > 1) {
		conninfo = argv[1];
	} else {
		conninfo = "";
	}

	pg.connect(conninfo, true);

	if (pg.check()) {
		std::cout << "CONNECTED" << std::endl;

		HTTPserver srv;

		srv.setupStatic();

		ok = srv.start();

		pg.disconnect();
	}

	return (ok ? 0 : -1);
}


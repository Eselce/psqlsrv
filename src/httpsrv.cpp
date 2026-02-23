
#include <iostream>

#include "dbconn.hpp"

#include "httpsrv.hpp"

static const std::string defaultip4 = "0.0.0.0";

HTTPserver::HTTPserver(void)
:	NiceService(false, false, false, false),
	m_pSrv(nullptr),
	m_rulescount(0)
{
	// HTTP
	m_pSrv = new httplib::Server();

	// HTTPS
//	m_pSrc = new httplib::SSLServer();
}

HTTPserver::HTTPserver(int port)
:	NiceService(false, false, false, false),
	m_pSrv(nullptr),
	m_rulescount(0)
{
	// HTTP
	m_pSrv = new httplib::Server();

	// HTTPS
//	m_pSrc = new httplib::SSLServer();

	this->setupStatic();

	this->start(port);
}

HTTPserver::~HTTPserver(void)
{
}

bool HTTPserver::start(int port)
{
	std::cerr << "Server listening at port " << port << "..." << std::endl;

	bool ok = m_pSrv->listen(defaultip4, port);

	if (! ok)
	{
		std::cerr << "Error: Cannot start listener!" << std::endl;

		this->exit_nicely();
	}

	return ok;
}

void HTTPserver::stop(void)
{
	if (m_pSrv != nullptr) {
		std::cerr << "Stopping server..." << std::endl;
		httplib::Server *pSrv = m_pSrv;

		m_pSrv = nullptr;
		m_rulescount = 0;

		delete pSrv;

#if defined(_DEBUG)
		if (this->getVerbose()) {
			std::clog << "Deleted server: " << this << std::endl;
		}
#endif
		std::cerr << "Server stopped." << std::endl;
	}
}

void HTTPserver::disconnect(const bool force)
{
	if ((m_pSrv != nullptr) && (force || this->getDisconnectOnError() || this->getExitOnError())) {
		this->stop();
	}
}

int HTTPserver::setupStatic(void)
{
	this->addStaticGet("/hi", "Hello World!");
	this->addStaticGet("/newvid", "List of new VIDs...");
	this->addGet("/dyn.*", &HTTPserver::testhandler);

	std::cout << this->m_rulescount << " rules created..." << std::endl;

	return this->m_rulescount;
}

void HTTPserver::setDBConnection(DBconnection *conn)
{
	this->m_pConn = conn;
}

int HTTPserver::addStaticGet(const std::string &pattern, const std::string &out, const std::string &mimetype)
{
	const char *pattern_str = pattern.c_str();
	const char *out_str = strdup(out.c_str());
	const char *mimetype_str = strdup(mimetype.c_str());

	m_pSrv->Get(pattern_str, [out_str, mimetype_str](const httplib::Request &, httplib::Response &res) {
			res.set_content(out_str, mimetype_str);
		});

	this->m_rulescount++;

	std::cout << "Adding GET rule #" << this->m_rulescount << ": '" << pattern_str << "' => '" << out_str << "' [" << mimetype_str << "]" << std::endl;

	return this->m_rulescount;
}

int HTTPserver::addGet(const std::string &pattern, std::string (HTTPserver::*handler)(const httplib::Request &), const std::string &mimetype)
{
	const char *pattern_str = pattern.c_str();
	const char *mimetype_str = strdup(mimetype.c_str());

	m_pSrv->Get(pattern_str, [this, handler, mimetype_str](const httplib::Request &req, httplib::Response &res) {
			res.set_content((this->*handler)(req).c_str(), mimetype_str);
		});

	this->m_rulescount++;

	std::cout << "Adding GET rule #" << this->m_rulescount << ": '" << pattern_str << "' => " << "::*handler()" << " [" << mimetype_str << "]" << std::endl;

	return this->m_rulescount;
}

std::string HTTPserver::testhandler(const httplib::Request &req)
{
	// req.method			- GET
	// req.version			- HTTP/1.1
	// req.matched_route	- /dyn.*
	// req.target           - /dynamisch?test=4
	// req.path				- /dynamisch
	// req.body		        - (empty)
	// User-Agent: 			- Mozilla/5.0 (X11; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0
	// Accept: 				- text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
	// test (param):		- 4
	// test (header):		- 0 (empty)
	// test (param):		- 1 (4)
	// test (trailer):		- 0 (empty)
	// req.remote_addr		- 127.0.0.1
	// req.remote_port		- 55356
	std::string ret = req.method + " " + req.version + " " + req.path + " " + req.matched_route + " "
			+ req.target + " " + req.get_header_value("User-Agent") + " " + req.get_header_value("Accept") + " "
			+ std::to_string(req.get_header_value_count("test")) + " " + std::to_string(req.get_param_value_count("test")) + " "
			+ std::to_string(req.get_trailer_value_count("test")) + " "	+ req.get_param_value("test") + " "
			+ req.remote_addr + ":" + std::to_string(req.remote_port) + " -> " + req.body + "\n";
	std::string query = "SELECT * FROM " + req.get_param_value("test");
	std::string answer = m_pConn->getanswer(query.c_str(), "SELECT failed");

	ret += answer;

	return ret;
}


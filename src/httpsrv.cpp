
#include <iostream>

#include "httpsrv.hpp"

static const std::string defaultip4 = "0.0.0.0";

HTTPserver::HTTPserver(void)
:	m_pSrv(NULL),
	m_rulescount(0)
{
	// HTTP
	m_pSrv = new httplib::Server();

	// HTTPS
//	m_pSrc = new httplib::SSLServer();
}

HTTPserver::HTTPserver(int port)
:	m_pSrv(NULL),
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
	this->stop();
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
	httplib::Server *pSrv = m_pSrv;

	m_pSrv = NULL;
	m_rulescount = 0;

	delete pSrv;
}

int HTTPserver::setupStatic(void)
{
	this->addStaticGet("/hi", "Hello World!");
	this->addStaticGet("/newvid", "List of new VIDs...");
	this->addGet("/dyn.*", &HTTPserver::testhandler);

	std::cout << this->m_rulescount << " rules created..." << std::endl;

	return this->m_rulescount;
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

int HTTPserver::addGet(const std::string &pattern, const std::string (HTTPserver::*handler)(const httplib::Request &), const std::string &mimetype)
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

const std::string HTTPserver::testhandler(const httplib::Request &req)
{
	std::string ret = "";

	ret += req.method + ".";
	ret += req.path + ".";
       	ret += req.matched_route + ".";
	ret += req.target;

	return ret;
}

void HTTPserver::exit_nicely(void)
{
	this->stop();

	exit(1);
}


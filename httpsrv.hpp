
#pragma once

#include <iostream>

//#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"

static const int defaultport = 8080;
static const std::string textplain = "text/plain";

class HTTPserver
{
public:
	HTTPserver(void);

	HTTPserver(int port);

	virtual ~HTTPserver(void);

public:
	bool start(int port = defaultport);

	void stop(void);

	int setupStatic();

protected:
	int addStaticGet(const std::string &cmd, const std::string &out, const std::string &mimetype = textplain);

	int addGet(const std::string &pattern, const std::string (HTTPserver::*handler)(const httplib::Request &), const std::string &mimetype = textplain);

	const std::string testhandler(const httplib::Request &req);

	void exit_nicely(void);

protected:
	httplib::Server *m_pSrv;

	int m_rulescount;
};


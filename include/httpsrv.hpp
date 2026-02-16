
#pragma once

#include <iostream>

//#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"

#include "dbconn.hpp"

#include "nicesvc.hpp"

static const int defaultport = 8080;
static const std::string textplain = "text/plain";

class HTTPserver : public NiceService
{
public:
	HTTPserver(void);

	HTTPserver(int port);

	virtual ~HTTPserver(void);

public:
	bool start(int port = defaultport);

	void stop(void);

	int setupStatic();

	void setDBConnection(DBconnection *conn);

protected:
	int addStaticGet(const std::string &cmd, const std::string &out, const std::string &mimetype = textplain);

	int addGet(const std::string &pattern, const std::string (HTTPserver::*handler)(const httplib::Request &), const std::string &mimetype = textplain);

	const std::string testhandler(const httplib::Request &req);

	virtual void disconnect(const bool force = true);

protected:
	httplib::Server *m_pSrv;

	int m_rulescount;

	DBconnection *m_pConn;
};


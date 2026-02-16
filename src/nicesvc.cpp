
#include <iostream>

#include "nicesvc.hpp"

NiceService::NiceService(const bool disconnectOnError,
							const bool exitOnError,
							const bool blocking,
							const bool verbose)
:	m_disconnectOnError(disconnectOnError),
	m_exitOnError(exitOnError),
	m_blocking(blocking),
	m_verbose(verbose)
{
}

NiceService::~NiceService(void)
{
	this->disconnect();
}

void NiceService::setDisconnectOnError(const bool disconnectOnError)
{
	this->m_disconnectOnError = disconnectOnError;
}

void NiceService::setExitOnError(const bool exitOnError)
{
	this->m_exitOnError = exitOnError;
}

void NiceService::setBlocking(const bool blocking)
{
	this->m_blocking = blocking;
}

void NiceService::setVerbose(const bool verbose)
{
	this->m_verbose = verbose;
}

bool NiceService::getDisconnectOnError(void) const
{
	return this->m_disconnectOnError;
}

bool NiceService::getExitOnError(void) const
{
	return this->m_exitOnError;
}

bool NiceService::getBlocking(void) const
{
	return this->m_blocking;
}

bool NiceService::getVerbose(void) const
{
	return this->m_verbose;
}

void NiceService::disconnect(const bool /*force*/)
{
}

void NiceService::exit_nicely(void)
{
	if (this->m_disconnectOnError) {
		this->disconnect(false);
	}

	if (this->m_exitOnError) {
		std::exit(1);
	}
}


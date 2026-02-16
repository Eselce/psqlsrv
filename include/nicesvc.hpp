
#pragma once

class NiceService
{
public:
	NiceService(const bool disconnectOnError = false,
				const bool exitOnError = false,
				const bool blocking = false,
				const bool verbose = false);

	virtual ~NiceService(void);

public:
	virtual void setDisconnectOnError(const bool disconnectOnError);

	virtual void setExitOnError(const bool exitOnError);

	virtual void setBlocking(const bool blocking);

	virtual void setVerbose(const bool verbose);

	virtual bool getDisconnectOnError(void) const;

	virtual bool getExitOnError(void) const;

	virtual bool getBlocking(void) const;

	virtual bool getVerbose(void) const;

protected:
	virtual void disconnect(const bool force = true);

	virtual void exit_nicely(void);

protected:
	bool m_disconnectOnError;

	bool m_exitOnError;

	bool m_blocking;

	bool m_verbose;
};


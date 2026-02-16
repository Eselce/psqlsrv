
#pragma once

#include <string>

class DBanswer
{
public:
	DBanswer(void);

	virtual ~DBanswer(void);

public:
	virtual const std::string getanswer(void) const = 0;

protected:
};


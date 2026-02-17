
#pragma once

#include <string>

#include "dbparam.hpp"

class DBanswer
{
public:
	DBanswer(void);

	virtual ~DBanswer(void);

public:
	virtual const std::string getanswer(const DBparameterFormat format = FORMAT_TEXT) const = 0;

protected:
};


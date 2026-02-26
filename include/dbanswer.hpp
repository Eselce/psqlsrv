
#pragma once

#include <string>

#include "db_type.h"

#include "dbparam.hpp"

class DBanswer
{
public:
	DBanswer(void);

	virtual ~DBanswer(void);

public:
	virtual std::string getanswer(const DBparameterFormat format = FORMAT_TEXT) const = 0;

protected:
	virtual std::string gettexttable(void) const = 0;

	virtual std::string getbinarytable(void) const = 0;

protected:
};



#pragma once

#include <string>

#include "dbparam.hpp"

class DBstatement
{
public:
	DBstatement(const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~DBstatement(void);

	virtual void setName(const char *name);

	virtual const std::string getautoname(void) const;

	const char *getName(void) const;

	const char *getCommand(void) const;

	const int getNParams(void) const;

	const DBparameterType *getParamTypes(void) const;

protected:
	std::string m_name;

	const std::string m_command;

	const int m_nParams;

	const DBparameterType *m_paramTypes;
};


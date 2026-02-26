
#pragma once

#include <string>

#include "dbparam.hpp"

#include "dbanswer.hpp"

class DBstatement
{
public:
	DBstatement(DBconnection *conn, const std::string &command = "", const int nParams = 0, const DBparameterType *paramTypes = nullptr);

	virtual ~DBstatement(void);

	virtual void setName(const char *name);

	virtual std::string getautoname(void) const;

	const char *getName(void) const;

	const char *getCommand(void) const;

	int getNParams(void) const;

	const DBparameterType *getParamTypes(void) const;

	virtual void resize(int nFields);

	virtual void setNFields(int nFields);

	int getNFields(void) const;

	virtual const DBparameterType *getFieldTypes(void) const;

	virtual std::string getFieldName(const int field) const;

	virtual int getFieldNumber(const std::string fieldName) const;

	virtual void prepare(void);

	virtual const DBanswer *exec(const char *errmsg, const DBparameterFormat resultFormat);

	virtual const DBanswer *exec(const DBparameter &param, const char *errmsg, const DBparameterFormat resultFormat);

protected:
	virtual void calcFieldInfos(void);

	virtual void cleanFieldInfos(void);

protected:
	DBconnection *m_pConn;

	std::string m_name;

	const std::string m_command;

	const int m_nParams;

	const DBparameterType *m_paramTypes;

	bool m_isprepared;

	int m_nFields;

	DBparameterType *m_fieldTypes;

	std::string *m_fieldNames;
};


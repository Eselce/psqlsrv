
#pragma once

#include "pg_type.h"

#include "dbparam.hpp"

class PGparameter : public DBparameter
{
public:
	template <typename... Args>
	PGparameter(Args... args)
	:	DBparameter(args...)
	{
		this->bind(args...);
	}

	virtual ~PGparameter(void) override;

public:
	virtual int parse(const char *str = nullptr, const char delim = ',') override;

	virtual int parsevar(const signed int &value, const int pos, const char *str = nullptr) override;
	virtual int parsevar(const unsigned int &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const signed short int &value, const int pos, const char *str = nullptr) override;
	virtual int parsevar(const unsigned short int &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const signed long int &value, const int pos, const char *str = nullptr) override;
	virtual int parsevar(const unsigned long int &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const __int128_t &value, const int pos, const char *str = nullptr) override;//+
	virtual int parsevar(const __uint128_t &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const float &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const double &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const long double &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const bool &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const signed char &value, const int pos, const char *str = nullptr) override;//+
	virtual int parsevar(const unsigned char &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const char16_t &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const char32_t &value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const void *value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const std::time_t *value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const std::tm *value, const int pos, const char *str = nullptr) override;//+

	virtual int parsevar(const std::string &value, const int pos, const char *str = nullptr) override;

	virtual int parsevar(const char *value, const int pos, const char *str = nullptr) override;

	virtual const Oid *types(void) const override;

	virtual std::string to_string(void) const override;

protected:
	virtual void bindany(const void *value, const int pos, const Oid type, const int length = 0, const DBparameterFormat format = FORMAT_BINARY) override;

	virtual void bindvar(const signed int &value, const int pos) override;
	virtual void bindvar(const unsigned int &value, const int pos) override;

	virtual void bindvar(const signed short int &value, const int pos) override;
	virtual void bindvar(const unsigned short int &value, const int pos) override;

	virtual void bindvar(const signed long int &value, const int pos) override;
	virtual void bindvar(const unsigned long int &value, const int pos) override;

	virtual void bindvar(const __int128_t &value, const int pos) override;//
	virtual void bindvar(const __uint128_t &value, const int pos) override;//

	virtual void bindvar(const float &value, const int pos) override;

	virtual void bindvar(const double &value, const int pos) override;

	virtual void bindvar(const long double &value, const int pos) override;//

	virtual void bindvar(const bool &value, const int pos) override;//

	virtual void bindvar(const signed char &value, const int pos) override;//
	virtual void bindvar(const unsigned char &value, const int pos) override;//

	virtual void bindvar(const char16_t &value, const int pos) override;//

	virtual void bindvar(const char32_t &value, const int pos) override;//

	virtual void bindvar(const void *value, const int pos) override;//

	virtual void bindvar(const std::time_t *value, const int pos) override;//

	virtual void bindvar(const std::tm *value, const int pos) override;//

	virtual void bindvar(const std::string &value, const int pos) override;

	virtual void bindvar(const char *value, const int pos) override;

protected:
};


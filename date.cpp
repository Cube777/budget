#include "date.h"

#define SEC_IN_DAY 86400

date_class::date_class()
{
	time_t tt = time(NULL);
	t = *localtime(&tt);
}

date_class::date_class(std::string str)
{
	t.tm_mday = std::stoi(str.substr(0, str.find('/')));
	str.erase(0, str.find('/') + 1);
	t.tm_mon = std::stoi(str.substr(0, str.find('/'))) - 1;
	str.erase(0, str.find('/') + 1);
	t.tm_year = std::stoi(str) - 1900;
}

date_class::date_class(int day, int month, int year)
{
	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
	t.tm_mday = day;
	t.tm_mon = month - 1;
	t.tm_year = year - 1900;
}

tm* date_class::gtm()
{
	return &t;
}

int date_class::day()
{
	return t.tm_mday;
}

int date_class::month()
{
	return t.tm_mon + 1;
}

int date_class::year()
{
	return t.tm_year + 1900;
}

std::string date_class::tostr()
{
	std::string tmp;

	tmp += dnum(day()) + "/";
	tmp += dnum(month()) + "/";
	tmp += std::to_string(year());
	return tmp;
}

int date_class::operator-(date_class d)
{
	return int(difftime(mktime(&t), mktime(d.gtm())) / SEC_IN_DAY);
}

bool date_class::operator<(date_class d)
{
	return (mktime(&t) < mktime(d.gtm()));
}

bool date_class::operator>(date_class d)
{
	return (mktime(&t) > mktime(d.gtm()));
}

std::string date_class::dnum(int num)
{
	std::string tmp = std::to_string(num);
	if (tmp.length() == 1)
		tmp = "0" + tmp;
	return tmp;
}

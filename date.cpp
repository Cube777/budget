#include "date.h"

#define SEC_IN_DAY 86400

std::string date_class::def_date;

date_class::date_class()
{
	if (!def_date.empty()) {
		set_str(def_date);
		return;
	}

	time_t tt = time(NULL);
	t = *localtime(&tt);

	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
}

date_class::date_class(tm time) :
	t(time)
{
}

date_class::date_class(std::string str)
{
	set_str(str);
}

date_class::date_class(int day, int month, int year)
{
	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
	t.tm_mday = day;
	t.tm_mon = month - 1;
	t.tm_year = year - 1900;

	time_t tt = mktime(&t);
	t = *localtime(&tt);
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

int date_class::wday()
{
	return t.tm_wday;
}

int date_class::mdays()
{
	switch (t.tm_mon) {
		case 1 : return (__isleap(year()) ? 29 : 28); break;
		case 0 :
		case 2 :
		case 4 :
		case 6 :
		case 7 :
		case 9 :
		case 11: return 31; break;
		default: return 30; break;
	}
}

std::string date_class::tostr()
{
	std::string tmp;

	tmp += dnum(day()) + "/";
	tmp += dnum(month()) + "/";
	tmp += std::to_string(year());
	return tmp;
}

void date_class::set_str(std::string str)
{
	t.tm_mday = std::stoi(str.substr(0, str.find('/')));
	str.erase(0, str.find('/') + 1);
	t.tm_mon = std::stoi(str.substr(0, str.find('/'))) - 1;
	str.erase(0, str.find('/') + 1);
	t.tm_year = std::stoi(str) - 1900;

	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;

	time_t tt = mktime(&t);
	t = *localtime(&tt);
}

date_class date_class::operator+(int days)
{
	time_t ep = mktime(&t);
	ep += days * SEC_IN_DAY;
	return date_class(*localtime(&ep));
}

void date_class::operator+=(int days)
{
	time_t ep = mktime(&t);
	ep += days * SEC_IN_DAY;
	t = *localtime(&ep);
}

date_class date_class::operator-(int days)
{
	return operator+(days * -1);
}

void date_class::operator-=(int days)
{
	operator+=(days * -1);
}

int date_class::operator-(date_class d)
{
	return int(difftime(mktime(&t), mktime(d.gtm())) / SEC_IN_DAY);
}

bool date_class::operator<(date_class d)
{
	return (mktime(&t) < mktime(d.gtm()));
}

bool date_class::operator<=(date_class d)
{
	return (mktime(&t) <= mktime(d.gtm()));
}

bool date_class::operator>(date_class d)
{
	return (mktime(&t) > mktime(d.gtm()));
}

bool date_class::operator>=(date_class d)
{
	return (mktime(&t) >= mktime(d.gtm()));
}

std::string date_class::dnum(int num)
{
	std::string tmp = std::to_string(num);
	if (tmp.length() == 1)
		tmp = "0" + tmp;
	return tmp;
}

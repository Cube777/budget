#include "date.h"

#define SEC_IN_DAY 86400

date_class::date_class()
{
}

date_class::date_class(int year, int month, int day)
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

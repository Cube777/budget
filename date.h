#pragma once

#include "time.h"

class date_class
{
public:
	date_class(int year, int month, int day);
	tm* gtm();

	int operator- (date_class d);
	bool operator< (date_class d);
	bool operator> (date_class d);

private:
	tm t;
};

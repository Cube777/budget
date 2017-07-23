#pragma once

#include "time.h"

class date_class
{
	public:
		date_class();
		date_class(int day, int month, int year);
		tm* gtm();

		int day();
		int month();
		int year();

		int operator- (date_class d);
		bool operator< (date_class d);
		bool operator> (date_class d);

	private:
		tm t;
};

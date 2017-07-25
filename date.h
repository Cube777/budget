#pragma once

#include "time.h"
#include <string>

class date_class
{
	public:
		date_class();
		date_class(std::string d);
		date_class(int day, int month, int year);
		tm* gtm();

		int day();
		int month();
		int year();

		std::string tostr();

		int operator- (date_class d);
		bool operator< (date_class d);
		bool operator> (date_class d);

	private:
		tm t;
		static std::string dnum(int num);
};

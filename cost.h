#pragma once

#include "date.h"

struct cost_class
{
	double cost;
	date_class date;
	bool exc;

	enum en_rec { nvr, wkly, mnly };
	en_rec rec;
};

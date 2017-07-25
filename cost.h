#pragma once

#include "date.h"

enum en_rec { nvr, wkly, mnly };

struct cost_class
{
	double cost;
	date_class date;
	bool exc;

	en_rec rec;
};

#include "mgr.h"
#include <iostream>

mgr_class::~mgr_class()
{
	for (auto itr : csts)
		for (auto i : itr.second)
			delete i;
}

void mgr_class::add(std::string name, date_class dt, double c)
{
	cost_class* cl = new cost_class;
	cl->date = dt;
	cl->cost = c;

	csts[name].push_back(cl);
}

void mgr_class::add(std::string name)
{
	date_class dc;
	int d, m, y;

	prompt("Day", d, dc.day());
	prompt("Month", m, dc.month());
	prompt("Year", y, dc.year());

	dc = date_class(d, m, y);

	double c;
	prompt("Cost:", c, 0.0);
	add(name, dc, c);
}

template <typename t1>
void mgr_class::prompt(std::string pr, t1 &var, t1 def)
{
	std::cout << pr << " [" << def << "]: ";
	std::string line;
	std::getline(std::cin, line);
	if (line.empty())
		var = def;
	else {
		if (std::is_same<t1, int>::value)
			var = int(std::stoi(line));
		else
			var = double(std::stod(line));
	}
}

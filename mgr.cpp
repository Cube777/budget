#include "mgr.h"
#include <iostream>
#include <iomanip>
#include <cmath>

mgr_class::~mgr_class()
{
	for (auto itr : csts)
		for (auto i : itr.second)
			delete i;
}

void mgr_class::add(std::string name, date_class dt, double c,
		cost_class::en_rec rec, bool ex)
{
	cost_class* cl = new cost_class;
	cl->date = dt;
	cl->cost = c;
	cl->rec = rec;
	cl->exc = ex;

	csts[name].push_back(cl);
}

void mgr_class::add(std::string name)
{
	if (name.empty()) {
		std::cout << "You must supply a name for the cost\n";
		return;
	}

	date_class dc;
	int d, m, y;

	prompt("Day", d, dc.day());
	prompt("Month", m, dc.month());
	prompt("Year", y, dc.year());

	dc = date_class(d, m, y);

	double c;
	prompt("Cost:", c, 0.0);
	add(name, dc, c, cost_class::nvr, false);
}

void mgr_class::exc(std::string name)
{
	if (name.empty()) {
		std::cout << "You must supply a name for exclusion\n";
		return;
	}
	list(name, true);
}

void mgr_class::list(std::string name, bool num)
{
	size_t n = 0;
	int c = 0;
	int l = -1;

	for (auto itr : csts) {
		if (!name.empty()) {
			if (itr.first != name)
				continue;
			else if (l < 0)
				l = int(log10(itr.second.size())) + 1;
		}
		n = std::max(itr.first.length(), n);
		for (auto itr : itr.second)
			c = std::max(int(log10(itr->cost)), c);
	}
	n++;
	c += 4;

	int cnt = 1;
	std::string nm;
	for (auto itr : csts) {
		nm = itr.first;
		if (!name.empty())
			if (nm != name)
				continue;
		for (auto itr : itr.second) {
			if (num)
				std::cout << std::right << cnt++ << ". ";
			std::cout << std::left << std::setw(n) << nm
				<< std::right << std::setw(c) << itr->cost << std::left;
			switch (itr->rec) {
			case cost_class::nvr : std::cout << ' ' << itr->date.tostr(); break;
			case cost_class::wkly : std::cout << " (Weekly)"; break;
			case cost_class::mnly : std::cout << " (Monthly)"; break;
			}
			std::cout << '\n';
		}
	}
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

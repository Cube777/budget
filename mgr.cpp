#include "mgr.h"

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

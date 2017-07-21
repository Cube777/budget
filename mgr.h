#pragma once

#include "cost.h"
#include <string>
#include <map>
#include <vector>

class mgr_class
{
	public:
		~mgr_class();
		void add(std::string name, date_class dt, double c);

	private:
		std::map<std::string, std::vector<cost_class*>> csts;
};

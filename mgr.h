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
		void add(std::string name);
		void list(std::string name);

	private:
		std::map<std::string, std::vector<cost_class*>> csts;

		template <typename t1>
		void prompt(std::string pr, t1 &var, t1 def);
};

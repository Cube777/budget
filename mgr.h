#pragma once

#include "cost.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>

class mgr_class
{
	public:
		mgr_class();
		~mgr_class();
		void add(std::string name, date_class dt, double c, en_rec rec, bool ex);
		void add(std::string name, date_class dt, double c);
		void add(std::string name);
		void addrec(std::string name);
		void exc(std::string name);
		void list(std::string name, bool num = false);

		void set_max_age(int age);
		void set_income(double v);
		void set_freq(en_rec e);
		void set_income();

		void status();

		void exp_prop(std::ofstream &file);
		void exp_data(std::ofstream &file);

	private:
		std::map<std::string, std::vector<cost_class*>> csts;

		template <typename t1>
		void prompt(std::string pr, t1 &var, t1 def);
		void clean();

		std::map<std::string, double> cur_costs();
		std::map<std::string, double> pln_costs();
		std::map<std::string, double> rec_costs();

		int max_age;
		en_rec freq;
		double inc;
};

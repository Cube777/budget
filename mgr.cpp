#include "mgr.h"
#include <iostream>
#include <iomanip>
#include <cmath>

mgr_class::mgr_class() :
	max_age(90),
	inc(0),
	freq(wkly)
{
	clean();
}

mgr_class::~mgr_class()
{
	clean();

	for (auto itr : csts)
		for (auto itr : itr.second)
			delete itr;
}

void mgr_class::add(std::string name, date_class dt, double c, en_rec rec,
		bool ex)
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
	add(name, dc, c, nvr, false);
}

void mgr_class::addrec(std::string name)
{
	if (name.empty()) {
		std::cout << "You must supply a name for the cost\n";
		return;
	}

	std::cout << "1. Weekly\n2. Monthly\n\nPlease choose a frequency for "
		<< "cost [1-2]: ";

	int ans;
	std::cin >> ans;
	if ( (ans < 1) || (ans > 2 ) ) {
		std::cout << "Invalid answer\n";
		return;
	}

	en_rec rec;
	switch (ans) {
		case 1 : rec = wkly; break;
		case 2 : rec = mnly; break;
	}

	double c;
	std::cout << "Cost: ";
	std::cin >> c;

	add(name, date_class(), c, rec, false);
	std::cin.ignore();
}

void mgr_class::exc(std::string name)
{
	if (name.empty()) {
		std::cout << "You must supply a name for exclusion\n";
		return;
	}
	if (csts.find(name) == csts.end())
		return;
	list(name, true);


	std::cout << "Choose a number [1-" << csts[name].size() << "]: ";
	int num;
	std::cin >> num;

	if ((num < 1) || (num > csts[name].size())) {
		std::cout << "Invalid option\n";
		return;
	}

	csts[name][num - 1]->exc = true;
	std::cin.ignore();
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
			case nvr : std::cout << ' ' << itr->date.tostr(); break;
			case wkly : std::cout << " (Weekly)"; break;
			case mnly : std::cout << " (Monthly)"; break;
			}
			std::cout << '\n';
		}
	}
}

void mgr_class::cur(bool rec)
{
	auto cst = (rec ? rec_costs() : cur_costs());
	int n = 0, c = 0;
	double total = 0;

	for (auto itr : cst) {
		n = std::max(n, int(itr.first.size()));
		c = std::max(c, int(log10(abs(itr.second))));
		total += itr.second;
	}

	n = std::max(n, int(std::string("Total: ").size()));
	c = std::max(c, int(log10(abs(total))));

	c += 5;

	for (auto itr : cst) {
		std::cout << std::left << std::setw(n) << itr.first
			<< std::right << std::setw(c) << itr.second;
		if (total != 0)
			std::cout << ' ' << std::setw(7) << itr.second / total * 100 << "%";
		std::cout << '\n';
	}

	std::cout << '\n' << std::left << std::setw(n) << "Total: "
		<< std::right << std::setw(c) << total << '\n';
}

void mgr_class::set_max_age(int age)
{
	max_age = age;
}

void mgr_class::set_income(double v)
{
	inc = v;
}

void mgr_class::set_freq(en_rec e)
{
	freq = e;
}

void mgr_class::set_income()
{
	std::cout << "1. Weekly\n2. Monthly\n\nPlease choose a frequency for "
		<< "your income [1-2]: ";

	int ans;
	std::cin >> ans;
	if ( (ans < 1) || (ans > 2 ) ) {
		std::cout << "Invalid answer\n";
		return;
	}

	switch (ans) {
		case 1 : freq = wkly; break;
		case 2 : freq = mnly; break;
	}

	std::cout << "Enter income amount: ";
	std::cin >> inc;
}

void mgr_class::status()
{
	auto curs = cur_costs();
	auto plns = pln_costs(curs);
	auto recs = rec_costs();

	double cur = 0;
	double pln = 0;
	double rec = 0;

	for (auto itr : curs)
		cur += itr.second;
	for (auto itr : plns)
		pln += itr.second;
	for (auto itr : recs)
		rec += itr.second;

	cur += rec;
	pln += rec;

	int w = int(std::max(log10(abs(cur)), log10(abs(pln))));
	w = std::max(int(log10(abs(rec))), w) + 5;;
	int b = int(std::max(log10(abs(inc - cur)), log10(abs(inc - pln)))) + 5;

	std::cout << "Recurring cost:" << std::right << std::setw(w) << rec << '\n'
		<< "Current cost:  " << std::right << std::setw(w) << cur
		<< std::left << "\tCurrent balance:" << std::right << std::setw(b)
		<< inc - cur << '\n' << std::left << "Planned cost:  " << std::right
		<< std::setw(w) << pln << std::left << "\tPlanned balance:"
		<< std::right << std::setw(b) << inc - pln << '\n';
}

void mgr_class::rank()
{
	auto pln = pln_costs();

	int w = 0;
	int c = 0;

	for (auto itr : pln) {
		w = std::max(int(itr.first.length()), w);
		c = std::max(int(log10(abs(itr.second))), c);
	}

	w++;
	c += 4;

	std::vector<std::string> names;
	std::vector<double> vals;
	for (auto itr : pln) {
		names.push_back(itr.first);
		vals.push_back(itr.second);
	}

	for (size_t i = 0; i < vals.size(); i++) {
		bool swap = false;
		for (size_t k = 1; k < vals.size(); k++) {
			if (vals[k-1] < vals[k]) {
				double tv = vals[k];
				vals[k] = vals[k-1];
				vals[k-1] = tv;

				std::string ts = names[k];
				names[k] = names[k-1];
				names[k-1] = ts;

				swap = true;
			}
		}
		if (!swap)
			break;
	}

	int n = log10(abs(names.size() + 1)) + 1;
	for (size_t i = 0; i < names.size(); i++) {
		std::cout << std::right << std::setw(n) << i + 1 << ". "
			<< std::left << std::setw(w) << names[i] << ' '
			<< std::right << std::setw(c) << vals[i] << '\n';
	}
}

std::vector<std::string> mgr_class::get_names()
{
	std::vector<std::string> ret;
	for (auto itr : csts)
		ret.push_back(itr.first);

	return ret;
}

void mgr_class::exp_prop(std::ofstream &file)
{
	file << "max_age=" << max_age << '\n';
	file << "income_value=" << inc << '\n';
	file << "income_freq=" << freq << '\n';
}

void mgr_class::exp_data(std::ofstream &file)
{
	for (auto itr : csts)
		for (auto cst : itr.second)
			file << itr.first << ' ' << cst->cost << ' ' << cst->date.tostr()
				<< ' ' << cst->rec << ' ' << cst->exc << '\n';
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

void mgr_class::clean()
{
	date_class today;
	bool rm;
	for (auto itr : csts) {
		size_t pos = 0;
		for (auto cst : itr.second) {
			rm = false;
			if (today - cst->date > max_age)
				rm = true;

			// More tests

			if (rm) {
				auto i = itr.second.begin();
				std::advance(i, pos);
				itr.second.erase(i);
			} else
				pos++;
		}
	}
}

cost_map mgr_class::cur_costs()
{
	date_class begin, end;

	if (freq == wkly) {
		begin -= (begin.wday() == 0 ? 6 : begin.wday() - 1);
		end = begin + 6;
	} else if (freq == mnly) {
		begin -= begin.day() - 1;
		end += end.mdays() - end.day();
	}

	cost_map ret;

	double total;
	for (auto itr : csts) {
		total = 0;
		for (auto i : itr.second) {
			if ((i->rec == nvr) && ((i->date >= begin) && (i->date <= end)))
				total += i->cost;
		} if (total != 0)
			ret[itr.first] = total;
	}
	return ret;
}

cost_map mgr_class::pln_costs()
{
	cost_map tmp;
	return pln_costs(tmp);
}

cost_map mgr_class::pln_costs(cost_map &cc)
{
	int days = 0;
	if (freq == wkly)
		days = 7;
	else if (freq == mnly)
		days = date_class().mdays();

	cost_map ret;
	date_class begin, end;
	double total;
	double tmp;
	for (auto itr : csts) {
		if (itr.second.size() < 2) {
			if (cc.find(itr.first) != cc.end()) {
				ret[itr.first] = cc[itr.first];
			}
			continue;
		}

		bool f = false;
		total = 0;
		tmp = 0;
		for (auto itr : itr.second) {
			if (itr->exc)
				continue;
			total += tmp;
			if (!f) {
				begin = itr->date;
				end = itr->date;
				f = true;
			}

			tmp = itr->cost;
			if (itr->date < begin)
				begin = itr->date;
			if (itr->date > end)
				end = itr->date;
		}
		ret[itr.first] = total / (end - begin) * days;
	}

	return ret;
}

cost_map mgr_class::rec_costs()
{
	int wim = 0;

	date_class dt;
	dt -= dt.day() - 1;
	int mon = dt.month();
	while (dt.month() == mon) {
		if (dt.wday() == 0)
			wim++;
		dt += 1;
	}

	cost_map ret;
	double total;

	for (auto itr : csts) {
		total = 0;
		for (auto itr : itr.second) {
			if (itr->rec == nvr)
				continue;
			if (itr->rec == freq)
				total += itr->cost;
			if ((itr->rec == wkly) && (freq == mnly))
				total += itr->cost * wim;
			if ((itr->rec == mnly) && (freq == wkly))
				total += itr->cost / wim;
		}

		if (total != 0)
			ret[itr.first] = total;
	}

	return ret;
}

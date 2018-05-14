#include "parser.h"
#include <fstream>

parser::~parser()
{
	std::ofstream file(get_fname());

	// Export properties
	mgr.exp_prop(file);

	file << "START\n";

	// Export data
	mgr.exp_data(file);

	file.close();
}

bool parser::parse()
{
	std::ifstream f(get_fname());

	if (!f.is_open()) {
		crf();
		f.open(get_fname());
		if (!f.is_open())
			return false;
	}

	std::string line, val;

	while (f >> line) {
		if (line == "START")
			break;
		size_t pos = line.find('=');
		if (pos == std::string::npos)
			break;
		val = line.substr(pos + 1);
		line.erase(pos);

		if (line == "max_age")
			mgr.set_max_age(std::stoi(val));
		if (line == "income_value")
			mgr.set_income(std::stod(val));
		if (line == "income_freq")
			mgr.set_freq(en_rec(std::stoi(val)));
	}

	std::string name, date;
	double cost;
	bool exc;
	int rec;
	while (f >> name >> cost >> date >> rec >> exc)
		mgr.add(name, date_class(date), cost, en_rec(rec), exc);

	return true;
}

std::string parser::get_fname()
{
	if (!fname.empty())
		return fname;
	fname = getenv("HOME");
	fname += "/.budget";
	return fname;
}

void parser::crf()
{
	std::ofstream f(get_fname());
	f.close();
}

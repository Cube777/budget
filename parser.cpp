#include "parser.h"
#include <fstream>

bool parser::parse()
{
	std::ifstream f(".budget");

	if (!f.is_open())
		return false;

	std::string name, date, rec;
	double cost;
	bool exc;
	cost_class::en_rec rec_en;
	while (f >> name >> cost >> date >> rec >> exc) {
		int d = std::stoi(date.substr(0, date.find('/')));
		date.erase(0, date.find('/') + 1);
		int m = std::stoi(date.substr(0, date.find('/')));
		date.erase(0, date.find('/') + 1);
		int y = std::stoi(date);

		if (rec == "nvr")
			rec_en = cost_class::nvr;
		else if (rec == "wkly")
			rec_en = cost_class::wkly;
		else if (rec == "mnthly")
			rec_en = cost_class::mnly;

		mgr.add(name, date_class(d, m, y), cost, rec_en, exc);
	}
	return true;
}

#include "parser.h"
#include <fstream>

bool parser::parse()
{
	std::ifstream f(".budget");

	if (!f.is_open())
		return false;

	std::string name, date;
	double cost;
	bool rec, exc;
	while (f >> name >> cost >> date >> rec >> exc) {
		int d = std::stoi(date.substr(0, date.find('/')));
		date.erase(0, date.find('/') + 1);
		int m = std::stoi(date.substr(0, date.find('/')));
		date.erase(0, date.find('/') + 1);
		int y = std::stoi(date);

		mgr.add(name, date_class(y, m, d), cost);
	}
	return true;
}

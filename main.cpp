#include <iostream>
#include <iomanip>
#include <regex>
#include "parser.h"

#define CNL "> "
#define NL "\n> "
#define CLN "\33[2K\r"

void stty(bool show = true, bool raw = false)
{
	std::string s = "stty ";
	if (!show)
		s += "-";
	s += "echo ";
	if (!raw)
		s += "-";
	s += "raw";
	system(s.c_str());
}

bool run(std::string s, parser &pr)
{
	std::string args;
	size_t pos = s.find(' ');
	if (pos != std::string::npos) {
		args = s.substr(pos + 1);
		s.erase(pos);
	}

	bool f = true;
	if (s == "exit")
		return false;
	else if (s == "add")
		pr.mgr.add(args);
	else if (s == "addrec")
		pr.mgr.addrec(args);
	else if (s == "exc")
		pr.mgr.exc(args);
	else if (s == "list")
		pr.mgr.list(args);
	else if (s == "rank")
		pr.mgr.rank();
	else if (s == "income")
		pr.mgr.set_income();
	else if (s == "status")
		pr.mgr.status();
	else
		f = false;

	if (!f)
		std::cout << "Unknown command: \"" << s << "\"\n";
	return true;
}

std::string complete(std::string s, parser &pr)
{
	std::string com, arg;
	size_t pos = s.find(' ');
	com = s.substr(0, pos);
	arg = s.substr(pos + 1);

	bool f = (com == s);
	std::vector<std::string> comps;

	if (f) {
		arg.clear();

		comps.push_back("exit");
		comps.push_back("add");
		comps.push_back("addrec");
		comps.push_back("exc");
		comps.push_back("list");
		comps.push_back("rank");
		comps.push_back("income");
		comps.push_back("status");
	} else
		comps = pr.mgr.get_names();

	std::regex rgx("(" + (f ? com : arg) + ")"+ "(.*)");

	std::vector<std::string> m;
	for (auto itr : comps)
		if (std::regex_match(itr, rgx))
			m.push_back(itr);

	if (m.empty())
		return s;
	if (m.size() == 1)
		return (f ? m[0] : com + " " + m[0]);

	for (auto itr : m)
		std::cout << "\n\r" << itr;
	std::cout << "\n\r";

	std::string tmp;

	pos = 0;
	while (true) {
		if (pos >= m[0].size())
			break;
		char c = m[0][pos];
		bool f = true;
		for (auto itr : m)
			if ((pos >= itr.size()) || (itr[pos] != c)) {
				f = false;
				break;
			}

		if (!f)
			break;
		tmp += c;
		pos++;
	}

	return (f ? tmp : com + " " + tmp);
}

void cli(parser &pr)
{
	std::cout << NL;
	stty(false, true);
	std::string line;
	char c;
	while (true) {
		std::cin.get(c);

		if (c == '\r') {
			stty();
			std::cout << "\n\r";
			bool res = run(line, pr);
			std::cout << "\n\r";
			if (!res)
				return;
			std::cout << CNL;
			stty(false, true);
			line.clear();
			continue;
		}

		if (c == '\t') {
			line = complete(line, pr);
			std::cout << CLN << CNL << line;
			continue;
		}

		if (c == 127) {
			if (!line.empty())
				line.pop_back();
			std::cout << CLN << CNL << line;
			continue;
		}

		std::cout << c;
		line += c;
	}
}

int main()
{
	std::cout << std::fixed << std::setprecision(2);
	parser pr;
	if (!pr.parse())
		return EXIT_FAILURE;

	cli(pr);
	return 0;
}

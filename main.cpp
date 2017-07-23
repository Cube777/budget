#include <iostream>
#include <iomanip>
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
	else if (s == "list")
		pr.mgr.list(args);
	else
		f = false;

	if (!f)
		std::cout << "Unknown command: \"" << s << "\"\n";
	return true;
}

std::string complete(std::string s)
{
	return s;
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
			line = complete(line);
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

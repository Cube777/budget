#include <iostream>
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
		s.substr(0, pos);
		s.erase(pos);
	}

	if (s == "exit")
		return false;

	if (s == "add") {
		pr.mgr.add(args);

		return true;
	}

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
	parser pr;
	if (!pr.parse())
		return EXIT_FAILURE;

	cli(pr);
	return 0;
}

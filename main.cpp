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

bool run(std::string s)
{
	if (s == "exit")
		return false;
}

std::string complete(std::string s)
{
	return s;
}

void cli()
{
	std::cout << NL;
	stty(false, true);
	std::string line;
	char c;
	while (true) {
		std::cin.get(c);

		if (c == '\r') {
			stty();
			bool res = run(line);
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

		line += c;
		std::cout << c;
	}
}

int main()
{
	parser pr;
	if (!pr.parse())
		return EXIT_FAILURE;

	cli();
	return 0;
}

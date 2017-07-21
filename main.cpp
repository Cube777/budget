#include <iostream>
#include "parser.h"

#define NL "\n> "

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

void run(std::string s)
{

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
			run(line);
			std::cout << NL;
			stty(false, true);
			line.clear();
			continue;
		}

		if (c == '\t')
			continue;

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

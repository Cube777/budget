#pragma once

#include "mgr.h"

class parser
{
	public:
		~parser();

		bool parse();
		mgr_class mgr;

	private:
		std::string fname;
		std::string get_fname();
		void crf();
};

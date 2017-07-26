#pragma once
#include <string_view>
#include "typename.h"


class variable{
public:
	variable() = delete;
	void setType() {
		//do stuff
	};

private:
	const Typename m_type;
	std::string jsonVals = "";
	/*
	thingy0: val0,
	thingy1: {thingy2:val1,
			  thingy3:val3},
	*/
};

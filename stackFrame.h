#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "variable.h"
#include <unordered_set>
#include "typename.h"
#include "function.h"

class stackFrame;


class line {
public:
	line(stackFrame& t_parent,std::string t_code) :m_parentFrame(t_parent),m_code(t_code){};
	void run();
private:
	stackFrame& m_parentFrame;
	std::string m_code;
};

class stackFrame {
public:
	stackFrame() = delete;
	stackFrame(stackFrame& t_parentFrame) :m_parentFrame(t_parentFrame){};
	void run();
	virtual bool hasType(std::string_view type) const;
	//void addVar(std::string Typename,);
protected:
	std::vector<stackFrame> m_frames;
	std::string m_code;
	stackFrame& m_parentFrame;
	std::unordered_map<std::string, Typename> m_types;
	std::unordered_map<std::string, function> m_functions;
};

class globalStackFrame :public stackFrame {
public:

protected:

};
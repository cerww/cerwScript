#pragma once
#include <vector>
#include <string>
#include "stackFrame.h"

struct functionArg {
	functionArg() = default;
	functionArg(Typename t_type, std::string t_name) :type(t_type),name(t_name){};
	Typename type;
	std::string name = "";
};

class function {
public:
	function() = default;
	function(std::string t_retType,std::vector<functionArg> t_sig,stackFrame* t_parent,const instructionSet& a) :
		m_retType(std::move(t_retType)),
		m_sig(std::move(t_sig)),
		m_parent(t_parent),
		orderOfInstructions(a){};
	const std::string& getRetType()const { return m_retType; };
	const std::vector<functionArg>& getSig()const { return m_sig; };
	variable run(const std::vector<variable>&);
private:
	const std::string m_retType;
	const std::vector<functionArg> m_sig;
	stackFrame* m_parent;
	const instructionSet& orderOfInstructions;
};


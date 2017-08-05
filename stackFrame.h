#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "variable.h"
#include <unordered_set>
#include <variant>
#include <memory>
#include "instruction.h"
#include "randomthings.h"
#include <iostream>

class function;

class stackFrame {
public:
	stackFrame() = delete;
	stackFrame(stackFrame* t_parentFrame,const instructionSet& t_orderOfExecution) :
		m_parentFrame(t_parentFrame),
		m_orderOfExecution(t_orderOfExecution){
	};
	stackFrame& run();
	virtual Typename getType(const std::string& type) const;
	//void addVar(std::string Typename,);
	virtual variable getVal(std::string&&);
	virtual variable getVal(const std::string&);
	virtual variable callFunction(const std::string& functionName, const std::vector<variable>& args);
	stackFrame& createVar(Typename type,const std::string& varName) {
		m_vars.insert(std::make_pair(varName, variable(type)));
		return *this;
	}
	stackFrame& createVar(const std::string& type, const std::string& varName) {
		m_vars.insert(std::make_pair(varName,variable(getType(type))));
		return *this;
	}
	stackFrame& setVar(const std::string& name, variable val);
	void addType(std::string);
protected:
	int i = 0;
	const instructionSet& m_orderOfExecution;
	stackFrame* m_parentFrame = nullptr;
	std::unordered_map<std::string, Typename> m_types;
	std::unordered_map<std::string, variable> m_vars;
	std::unordered_map<std::string, std::shared_ptr<function>> m_functions;
	//void runInstruction(const instruction&);
	template<typename instructionType>
	void runInstruction(const instructionType&);
	template<>
	void runInstruction<creatValInstruction>(const creatValInstruction&);
	template<>
	void runInstruction<setValInstruc>(const setValInstruc&);
	template<>
	void runInstruction<callFunctionInstruction>(const callFunctionInstruction&);
	template<>
	void runInstruction<ifStatmentInstruction>(const ifStatmentInstruction&);
	template<>
	void runInstruction<addFunctionInstruction>(const addFunctionInstruction&);
	template<>
	void runInstruction<instructionSet>(const instructionSet&);
	template<>
	void runInstruction<returnValInstruction>(const returnValInstruction&);
	template<>
	void runInstruction<printInstruction>(const printInstruction&);
};

class globalStackFrame:public stackFrame {
public:
	globalStackFrame(const instructionSet& a) :stackFrame(this, a) {
		Typename::addType("int", {});
		addType("int");
	};
	variable getVal(std::string&&)override final ;
	variable getVal(const std::string&)override final;
	Typename getType(const std::string& type)const override final;
	variable callFunction(const std::string& functionName, const std::vector<variable>& args) override final;
};


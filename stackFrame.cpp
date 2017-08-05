#include "stackFrame.h"
#include "function.h"
#include <iostream>

stackFrame& stackFrame::run() {
	for (i = 0; i < m_orderOfExecution.instructions.size();++i) {
		std::visit([&](const auto& in) {
			runInstruction(in);
		},m_orderOfExecution.instructions[i]);
	}return *this;
}

Typename stackFrame::getType(const std::string & type) const{
	auto it = m_types.find(type);
	if (it == m_types.end()) {
		return m_parentFrame->getType(type);
	}return it->second;
}

variable stackFrame::getVal(std::string&& varName) {
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass;
		varsToPass.reserve(3);
		for (size_t start = varName.find('(') + 1; varName[start] != ')';) {
			int notClosedBrakets = 0;
			size_t start2 = start;
			do {
				start2 = varName.find_first_of(",()", start2 + 1);
				if (varName[start2] == ')') {
					--notClosedBrakets;
				}
				else if (varName[start2] == '(') {
					++notClosedBrakets;
				}
				else {//,
					if (!notClosedBrakets) break;
				}
			} while (start2 < varName.size() && notClosedBrakets != -1);
			varsToPass.push_back(getVal(varName.substr(start, start2 - start - 1)));
			start = start2 + 1;
		}return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass);
	}
	auto it = m_vars.find(varName);
	if (it == m_vars.end()) {
		return m_parentFrame->getVal(varName);
	}return it->second;
}

variable stackFrame::getVal(const std::string & varName){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass;
		varsToPass.reserve(3);
		for (size_t start = varName.find('(') + 1; varName[start] != ')';) {
			int notClosedBrakets = 0;
			size_t start2 = start;
			do {
				start2 = varName.find_first_of(",()", start2 + 1);
				if (varName[start2] == ')') {
					--notClosedBrakets;
				}
				else if (varName[start2] == '(') {
					++notClosedBrakets;
				}
				else {//,
					if (!notClosedBrakets) break;
				}
			} while (start2 < varName.size() && notClosedBrakets != -1);
			varsToPass.push_back(getVal(varName.substr(start, start2 - start - 1)));
			start = start2 + 1;
		}return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass);
	}
	auto it = m_vars.find(varName);
	if (it == m_vars.end()) {
		return m_parentFrame->getVal(varName);
	}return it->second;
}

variable stackFrame::callFunction(const std::string& functionName, const std::vector<variable>& args) {
	auto it = m_functions.find(functionName);
	if (it != m_functions.end()) {
		return it->second->run(args);
	}return m_parentFrame->callFunction(functionName, args);
}

stackFrame& stackFrame::setVar(const std::string & name, variable val) {
	variable& i = m_vars.at(name);
	i.m_stuffs = std::move(val.m_stuffs);
	i.m_val = val.m_val;

	return *this;
}

void stackFrame::addType(std::string nameOfType){
	m_types.insert(std::make_pair(nameOfType, Typename{ nameOfType }));
}

template<typename instructionType>
void stackFrame::runInstruction(const instructionType &){
	*nullptr = 1;
}

template<>
void stackFrame::runInstruction(const creatValInstruction & thingy){
	createVar(thingy.Typename, thingy.varName);
}

template<>
void stackFrame::runInstruction(const setValInstruc & thingy){
	setVar(thingy.varName, getVal(thingy.toSetTo));
}

template<>
void stackFrame::runInstruction(const callFunctionInstruction & thingy){
	std::vector<variable> args;
	args.reserve(thingy.namesOfArgs.size());
	for (const auto& i : thingy.namesOfArgs) {
		args.push_back(getVal(i));
	}callFunction(thingy.functionName, args);
}

template<>
void stackFrame::runInstruction(const ifStatmentInstruction &thingy){
	if (getVal(thingy.thingy).m_val != 1) --i;	
}

template<>
void stackFrame::runInstruction(const addFunctionInstruction & thingy){
	std::vector<functionArg> functionArgs;
	functionArgs.reserve(thingy.functionSig.size());
	for (const auto& i : thingy.functionSig) {
		const auto a = splity(i,' ');
		functionArgs.emplace_back(getType(a[0]),a[1]);
	}m_functions.insert(std::make_pair(thingy.functionName, std::make_shared<function>(thingy.retValType, std::move(functionArgs), this, thingy.stuffs)));
	//std::cout << m_functions.size() << std::endl;
}

template<>
void stackFrame::runInstruction(const instructionSet & thingy){
	stackFrame a(this,thingy);
	a.run();
}

template<>
void stackFrame::runInstruction(const returnValInstruction & thingy){
	setVar("RET_VAL_EEEE", getVal(thingy.val));
	i = 213123;
}

template<>
void stackFrame::runInstruction(const printInstruction & thingy){//fix this
	std::cout << getVal(thingy.thingy).m_val;
}



//globalStackFrame here! :D

variable globalStackFrame::getVal(std::string && varName){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass;
		varsToPass.reserve(3);
		for (size_t start = varName.find('(') + 1; varName[start] != ')';) {
			int notClosedBrakets = 0;
			size_t start2 = start;
			do {
				start2 = varName.find_first_of(",()", start2 + 1);
				if (varName[start2] == ')') {
					--notClosedBrakets;
				}
				else if (varName[start2] == '(') {
					++notClosedBrakets;
				}
				else {//,
					if (!notClosedBrakets) break;
				}
			} while (start2 < varName.size() && notClosedBrakets != -1);
			varsToPass.push_back(getVal(varName.substr(start, start2 - start - 1)));
			start = start2 + 1;
		}return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass);
	}
	return m_vars[varName];
}

variable globalStackFrame::getVal(const std::string & varName){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass;
		varsToPass.reserve(3);
		for (size_t start = varName.find('(') + 1; varName[start] != ')';) {
			int notClosedBrakets = 0;
			size_t start2 = start;
			do {
				start2 = varName.find_first_of(",()", start2 + 1);
				if (varName[start2] == ')') {
					--notClosedBrakets;
				}else if (varName[start2] == '(') {				
					++notClosedBrakets;
				}else{//,
					if (!notClosedBrakets) break;					
				}
			} while (start2 < varName.size() && notClosedBrakets != -1);
			varsToPass.push_back(getVal(varName.substr(start, start2 - start - 1)));
			start = start2 + 1;
		}return callFunction(varName.substr(0, varName.find('(')-1), varsToPass);
	}
	try {
		int temp = std::stoi(varName);
		auto tempVar = variable(getType("int"));
		tempVar.m_val = temp;
		return tempVar;
	}catch(...){
		return m_vars[varName]; 
	}
}

Typename globalStackFrame::getType(const std::string & type) const{
	auto it = m_types.find(type);
	return it->second;
}

variable globalStackFrame::callFunction(const std::string & functionName, const std::vector<variable>& args){
	return m_functions.at(functionName)->run(args);
}




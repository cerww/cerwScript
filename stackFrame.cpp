#include "stackFrame.h"
#include "function.h"
//#include <iostream>

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

variable stackFrame::getVal(std::string&& varName, stackFrame* start) {
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass = start->splitVarsToVars(std::string_view(varName.data() + varName.find('('), varName.size() - varName.find('(')),start?start:this);
		return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass, start ? start : this);
	}
	auto it = m_vars.find(varName);
	if (it == m_vars.end()) {
		return m_parentFrame->getVal(varName, start ? start : this);
	}return it->second;
}

variable stackFrame::getVal(const std::string & varName, stackFrame* start){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass = start->splitVarsToVars(std::string_view(varName.data() + varName.find('('), varName.size() - varName.find('(')),start?start:this);
		return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass, start ? start : this);
	}
	auto it = m_vars.find(varName);
	if (it == m_vars.end()) {
		return m_parentFrame->getVal(varName,start?start:this);
	}return it->second;
}

variable stackFrame::callFunction(const std::string& functionName, const std::vector<variable>& args, stackFrame* start) {
	auto it = m_functions.find(functionName);
	if (it != m_functions.end()) {
		return it->second->run(args);
	}	
	return m_parentFrame->callFunction(functionName, args, start ? start : this); 
	
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

std::vector<variable> stackFrame::splitVarsToVars(std::string_view vars, stackFrame* start){
	std::vector<variable> retVal;
	int numOfNotClosedBrackets = 1;//start at 1,first char is '('
	size_t currentSpot = 1;//skip first char
	while (numOfNotClosedBrackets) {
		size_t spot = currentSpot;
		int num = 0;
		while (1) {
			spot = vars.find_first_of(",()", spot + 1);
			if (vars[spot] == '(') {
				++numOfNotClosedBrackets;
				++num;
			}else if (vars[spot] == ')') { 
				--num;
				--numOfNotClosedBrackets;
				if (num < 0)break;
			}else{//,
				if (!num) break;
			}
		}retVal.push_back(start->getVal(std::string(vars.data() + currentSpot, spot - currentSpot)));//spot will now be at a comma or ennd of thingy
		//std::cout << std::string(vars.data() + currentSpot, spot - currentSpot) << std::endl;
		currentSpot = spot + 1;
	};
	return std::move(retVal);
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

variable globalStackFrame::getVal(std::string && varName, stackFrame* start){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass = start->splitVarsToVars(std::string_view(varName.data() + varName.find('('), varName.size() - varName.find('(')),start);
		return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass, start);
	}try {
		int temp = std::stoi(varName);
		auto tempVar = variable(getType("int"));
		tempVar.m_val = temp;
		return tempVar;
	}
	catch (...) {
		size_t isArithmatic = varName.find_first_of("/*+-");
		if (isArithmatic != std::string::npos) {
			variable tempVar(getType("int"));
			tempVar.m_val = start->getVal(varName.substr(0, isArithmatic)).m_val;
			switch (varName[isArithmatic]) {
			case '*':
				tempVar.m_val *= start->getVal(varName.substr(isArithmatic + 1)).m_val;
			case '-':
				tempVar.m_val -= start->getVal(varName.substr(isArithmatic + 1)).m_val;
			case '+':
				tempVar.m_val += start->getVal(varName.substr(isArithmatic + 1)).m_val;
			case '/':
				tempVar.m_val /= start->getVal(varName.substr(isArithmatic + 1)).m_val;
			}return std::move(tempVar);
		}return m_vars[varName];
	}
}

variable globalStackFrame::getVal(const std::string & varName, stackFrame* start){
	if (varName[varName.size() - 1] == ')') {//function call
		std::vector<variable> varsToPass = start->splitVarsToVars(std::string_view(varName.data() + varName.find('('), varName.size() - varName.find('(')),start);
		return callFunction(varName.substr(0, varName.find('(') - 1), varsToPass);
	}
	try {
		int temp = eval(varName);
		auto tempVar = variable(getType("int"));
		tempVar.m_val = temp;
		return tempVar;
	}catch(...){
		size_t isArithmatic = varName.find_first_of("/*+-");
		if (isArithmatic != std::string::npos) {
			variable tempVar(getType("int"));
			tempVar.m_val = start->getVal(varName.substr(0,isArithmatic)).m_val;
			std::cout << varName.substr(0, isArithmatic).size() << std::endl;
			switch (varName[isArithmatic]) {
			case '*':
				tempVar.m_val *= start->getVal(varName.substr(isArithmatic + 1)).m_val;
				break;
			case '-':
				tempVar.m_val -= start->getVal(varName.substr(isArithmatic + 1)).m_val;
				break;
			case '+':
				tempVar.m_val += start->getVal(varName.substr(isArithmatic + 1)).m_val;
				break;
			case '/':
				tempVar.m_val /= start->getVal(varName.substr(isArithmatic + 1)).m_val;
				break;
			}return std::move(tempVar);
		}		
	}return m_vars[varName];
}

Typename globalStackFrame::getType(const std::string & type) const{
	auto it = m_types.find(type);
	return it->second;
}

variable globalStackFrame::callFunction(const std::string & functionName, const std::vector<variable>& args, stackFrame* start){
	if (functionName == "add") {
		variable tempVar(getType("int"));
		tempVar.m_val = args[0].m_val+ args[1].m_val;
		return tempVar;
	}else if(functionName=="multiply"){
		variable tempVar(getType("int"));
		tempVar.m_val = args[0].m_val * args[1].m_val;
		return tempVar;	
	}else if (functionName == "sub") {
		variable tempVar(getType("int"));
		tempVar.m_val = args[0].m_val - args[1].m_val;
		return tempVar;	
	}else if (functionName == "divide") {
		variable tempVar(getType("int"));
		tempVar.m_val = args[0].m_val / args[1].m_val;
		return tempVar;
	}
	return m_functions.at(functionName)->run(args);
}




#pragma once
#include <string>
#include <vector>
#include <variant>

enum class instructionType {
	SET_VAR,
	CREATE_VAR,
	FUNCTION_CALL,
	GET_VAR,//<-wat does this one do
	RETURN_VAL,
	PRINT_STUFF,
	IF_STATEMENT,
	ADD_FUNCTION
};


/*
a = f(b);->setVal(a,f(b));



*/
class stackFrame;
class instructionSet;
struct addFunctionInstruction;

struct instruction_base {
	instruction_base() = default;
};

class instruction {
public:
	instruction() = default;
	std::vector<std::string> stuffs;
private:
	instructionType m_type;

};

struct setValInstruc {
	setValInstruc() = default;
	std::string varName = "";
	std::string toSetTo = "";
};

struct creatValInstruction {
	creatValInstruction() = default;
	std::string Typename = "";
	std::string varName = "";
};//int u = 23; gets parsed into createVar and setVar

struct ifStatmentInstruction{
	std::string thingy = "";
};

struct printInstruction {
	std::string thingy = "";
};

struct callFunctionInstruction{
	std::string functionName;
	std::vector<std::string> namesOfArgs;
};

struct returnValInstruction{
	std::string val;
};

using instructionsVariant = std::variant<creatValInstruction, ifStatmentInstruction, setValInstruc, callFunctionInstruction, instructionSet, returnValInstruction, printInstruction, addFunctionInstruction>;

class instructionSet {
public:
	instructionSet() = default;
	template<class T,typename ... args>
	void addInstruction(args... Args);
	std::vector<instructionsVariant> instructions = {};
};

struct addFunctionInstruction {
	std::string retValType;
	std::string functionName;
	std::vector<std::string> functionSig;
	instructionSet stuffs;
};

template<class T, typename ... args>
void instructionSet::addInstruction(args... Args) {
	instructions.push_back(T{ std::forward<args>(Args)... });
}
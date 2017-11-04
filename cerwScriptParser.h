#pragma once
#include "instruction.h"
#include "stackFrame.h"
#include "randomThings.h"
#include "svstrStuffs.h"

void turnNewLineTabIntoSpace(std::string& code);
std::string removeUnneededSpaces(std::string code);
void removeMultipleInARow(std::string& code);
void replaceOperators(std::string& code);

class parser {
public:
	parser() = default;
	globalStackFrame getStuffs(std::string code) {
		code = removeUnneededSpaces("{" + code + "}");
		b = parseStuff(std::string_view(code.c_str(), code.size()));
		return globalStackFrame(b);
	}
private:
	instructionSet parseStuff(std::string_view);
	instructionSet b;

};

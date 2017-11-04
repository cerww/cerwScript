#include "cerwScriptParser.h"

std::string removeUnneededSpaces(std::string code){

	turnNewLineTabIntoSpace(code);

	removeMultipleInARow(code);

	const std::string_view stuffs = "";

	size_t currentSpot = 1;

	while (currentSpot < code.size()-1) {
		if (code[currentSpot - 1] == ' ') 
			code.erase(currentSpot-- - 1, 1);
		if (code[currentSpot + 1] == ' ') 
			code.erase(currentSpot + 1, 1);
		currentSpot = code.find_first_of("{};()[]-+*/=,", currentSpot + 1);
	}return std::move(code);
}

void turnNewLineTabIntoSpace(std::string & code){
	for (auto& i : code)
		if (i == '\n' || i == '\t') 
			i = ' ';	
}

void removeMultipleInARow(std::string & code){
	for (size_t i = 1; i < code.size() - 1; ++i) //first and last char isnt a space
		if (code[i] == ' ') 
			while (code[i + 1] == ' ') 
				code.erase(i + 1, 1);

}

void replaceOperators(std::string & code){
	size_t currentRelevantPos = 0;

}

instructionSet parser::parseStuff(std::string_view code) {
	instructionSet retVal;
	int numOfNotClosedBrackets = 1;
	size_t currentSpot = 0;

	while (numOfNotClosedBrackets) {
		size_t nextSpot = code.find_first_of("{};", currentSpot + 1);
		//do stuff with [current,nextSpot)
		if (code[nextSpot] == '{') {
			if (nextSpot-currentSpot==1) {//random stack frame inside one
				retVal.addInstruction<instructionSet>(parseStuff(std::string_view(code.data()+nextSpot,code.size()-nextSpot)));
			}
			else //function or if
			{
				const std::string functionName(code.data() + currentSpot + 1, code.find('(', currentSpot + 1) - currentSpot - 1); 
				size_t firstBracket = code.find_first_of('(', currentSpot + 1);
				size_t end = firstBracket;
				int numOfBrackets = 1;
				while (numOfBrackets) {
					end = code.find_first_of("()", end + 1);
					if (code[end] == '(')
						++numOfBrackets;
					else
						--numOfBrackets;
				}
				if (functionName == "if") {					
					retVal.addInstruction<ifStatmentInstruction>(
						std::string(code.data() + firstBracket + 1, end - firstBracket - 1)
					);
					retVal.addInstruction<instructionSet>(
						parseStuff(std::string_view(code.data() + nextSpot, code.size() - nextSpot))
					);
				}else {
					const auto a = splity(functionName,' ');

					//auto tempe = splity(std::string(code.data() + firstBracket + 1, end - firstBracket - 1), ',');

					retVal.addInstruction<addFunctionInstruction>(
						a[0],
						a[1],
						splity(std::string(code.data() + firstBracket + 1, end - firstBracket - 1),','),
						parseStuff(std::string_view(code.data() + nextSpot, code.size() - nextSpot))
					);
				}
			}
			int numOfNotClosedBracksets = 1;
			while (numOfNotClosedBracksets) {
				nextSpot = code.find_first_of("{}", nextSpot + 1);
				if (code[nextSpot] == '}')
					--numOfNotClosedBracksets;
				else if (code[nextSpot] == '{')
					++numOfNotClosedBracksets;
			}
		}
		else if (code[nextSpot] == ';') {//line of code
			size_t imNotSureWhatToNameThis = code.find_first_of(" (=", currentSpot);
			if (code[imNotSureWhatToNameThis] == '=') {//set var
				retVal.addInstruction<setValInstruc>(
					std::string(code.data() + currentSpot + 1, imNotSureWhatToNameThis - currentSpot - 1), //var name
					std::string(code.data() + imNotSureWhatToNameThis + 1, nextSpot - imNotSureWhatToNameThis - 1) //new val
					);
			}else if (code[imNotSureWhatToNameThis] == '(') {//call function
				auto functionName = std::string(code.data() + currentSpot + 1, imNotSureWhatToNameThis - currentSpot - 1);
				if (functionName == "print") {
					retVal.addInstruction<printInstruction>(
						std::string(code.data() + imNotSureWhatToNameThis + 1, nextSpot - imNotSureWhatToNameThis - 2)
						);
				}else {
					retVal.addInstruction<callFunctionInstruction>(
						functionName,
						splity(std::string(code.data() + imNotSureWhatToNameThis + 1, nextSpot - imNotSureWhatToNameThis - 2), ',')
						);
				}
			}else if (code[imNotSureWhatToNameThis] == ' ') {//create var, adds to instructions
				const size_t other = code.find_first_of("=;", imNotSureWhatToNameThis+1);//= is setval too
				const std::string_view nameOfVar(code.data() + imNotSureWhatToNameThis + 1, other - imNotSureWhatToNameThis - 1);
				retVal.addInstruction<creatValInstruction>(
					std::string(code.data() + currentSpot + 1, imNotSureWhatToNameThis - currentSpot - 1),
					toStr(nameOfVar)
					);
				if (code[other]== '=') {//set val
					retVal.addInstruction<setValInstruc>(
						toStr(nameOfVar),
						std::string(code.data()+other+1,nextSpot - other - 1)
						);
				}
			}
		}
		else if (code[nextSpot] == '}') {
			--numOfNotClosedBrackets;
		}currentSpot = nextSpot;
	}return retVal;
}



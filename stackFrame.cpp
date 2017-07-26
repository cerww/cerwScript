#include "stackFrame.h"

void stackFrame::run() {
	
}

bool stackFrame::hasType(std::string_view type) const{
	if (m_types.find(type.data()) != m_types.end())
		return true;
	return m_parentFrame.hasType(type);
}

void line::run(){//;-;
	std::string_view thingyUntilFirstSpace(m_code.c_str(),m_code.find(' '));
	if (m_code.find(';')!=std::string::npos) {//crash

	}else if (m_parentFrame.hasType(thingyUntilFirstSpace)) {
		//std::string_view name(m_code.substr(thingyUntilFirstSpace.size(),));
		std::string namey = m_code.substr(thingyUntilFirstSpace.size()+1);//+1 puts it after the space
		std::string_view name(namey.c_str(),namey.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$0123456789"));

	}
}

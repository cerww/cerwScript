#pragma once
#include <vector>
#include <string>


class function {
public:
	function(std::string t_retType,std::vector<std::string> t_sig) :
		m_retType(t_retType),
		m_sig(std::move(t_sig)) {};
private:
	const std::string m_retType;
	const std::vector<std::string> m_sig;
};

#pragma once
#include <string_view>
#include "typename.h"
#include <unordered_map>
#include <memory>


class variable{
public:
	variable() = default;
	variable(Typename t_type) :m_type(std::move(t_type)) {
		if (m_type.name == "int")
			m_val = 0;
		else
		for(const auto& i: m_type.getFields()){
			m_stuffs.insert(std::make_pair(i.second,std::make_shared<variable>(i.first)));
		}
	};
	variable& getItem(const std::string& itemName) {
		return *m_stuffs.at(itemName);
	}
	const variable& getItem(const std::string& itemName) const{
		return *m_stuffs.at(itemName);
	}
private:
	Typename m_type;
	std::unordered_map <std::string, std::shared_ptr<variable>> m_stuffs;
	int m_val;//;-; its wasted most of the time D: rip 4 bytes ;-;
	friend class stackFrame;
	friend class globalStackFrame;
};

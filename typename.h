#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

class Typename {
public:
	Typename() = default;
	std::string name = "";
	const std::vector<std::pair<Typename, std::string>>& getFields() const{
		return typeFields.at(name);
	}
	static void addType(const std::string& name, std::vector<std::pair<Typename, std::string>> fields) {
		typeFields[name] = std::move(fields);
	}
private:
	static std::unordered_map<std::string, std::vector<std::pair<Typename, std::string>>> typeFields;
};

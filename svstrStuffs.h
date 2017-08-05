#pragma once
#include <string_view>

inline std::string toStr(std::string_view a) {
	return std::string(a.data(),a.size());
}

#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <fstream>
//that might be usefull and not in the std already

template<typename fn>
struct Not{
	Not(fn t_f) : m_function(std::move(t_f)) {}
	template<typename ... Args>
	decltype(auto) operator()(Args... args) {
		return !m_function(std::forward<Args>(args)...);
	}
private:
	fn m_function;
};//callable that is returns !fn(args...) :D

template<typename fn>
inline auto not(fn func) {
	return Not<fn>(std::forward<fn>(func));
}

inline std::vector<std::string> splity(const std::string& string,char letter) {
	if (!string.size())return {};
	std::vector<std::string> retVal;
	size_t currentA = 0;
	do {
		size_t spot = string.find(letter, currentA);
		if(spot!=currentA)
			retVal.push_back(string.substr(currentA, spot - currentA));
		currentA = spot + 1;
	}while(currentA);//std::string::npos + 1 = 0
	return retVal;
}

inline std::vector<uint64_t> Range(const uint64_t n) {
	std::vector<uint64_t> retVal(n);
	std::generate(retVal.begin(), retVal.end(), [awesome = 0]()mutable {return awesome++; });
	return retVal;
}

//I found this online
inline std::string getFileContentsa(const std::string& filePath) {
	std::string fileContents;
	std::ifstream file(filePath, std::ios::in);
	file.seekg(0, std::ios::end);
	int filesize = (int)file.tellg();
	file.seekg(0, std::ios::beg);
	filesize -= (int)file.tellg();
	fileContents.resize(filesize);
	file.read(&fileContents[0], filesize);
	file.close();
	return std::move(fileContents);
}
//iota for for loops
/*
template<typename T>
class iota {
public:
	iota(T t_thing) :m_thing(t_thing) {};
	iota(T t_thing, size_t t_max) :m_thing(t_thing), m_max(t_max) {};
	struct iterator {
		iterator(iota<T>& t_parent, size_t t_count) :m_parent(t_parent), m_count(t_count) {};
		iterator(iota<T>& t_parent) :m_parent(t_parent) {};
		iterator& operator++(int) {
			++m_count;
			m_parent.m_thing++;
			return *this;
		};
		iterator& operator++() {
			++m_count;
			++m_parent.m_thing;
			return *this;
		};
		bool operator!=(const iterator& other) {
			return m_count != other.m_count;
		}
		const T& operator*() {
			return m_parent.m_thing;
		}
	private:
		iota<T>& m_parent;
		size_t m_count = 0;
	};
	iterator begin() {
		return iterator(*this);
	}
	iterator end() {
		return iterator(*this, m_max);
	}
private:
	friend struct iterator;
	T m_thing;
	const size_t m_max = -1;//infinity(close enough, accually size_t::max)
};
*/
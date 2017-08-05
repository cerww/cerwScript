#include <iostream>
#include <unordered_map>
#include <string>
#include "cerwScriptParser.h"

int main() {

	std::string a = "int main(){\nint c = 3;print(c);}";
	auto k = parser();
	auto b = k.getStuffs(a);
	b.run();
	b.callFunction("main", {});
	int t;
	std::cin >> t;
	return 0;
}


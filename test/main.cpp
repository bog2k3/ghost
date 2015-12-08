#include "../common/strCompare.h"
#include <iostream>
#include <string>

int main() {
	std::string s1("macclesfield town");
	std::string s2("erzgebirge aue");
	std::string s3("macclesfield");

	auto r1 = StrComp(s1, s2).getStats();
	auto r2 = StrComp(s1, s3).getStats();

	std::cout << "a";
}

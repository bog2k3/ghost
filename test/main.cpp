#include <iostream>
#include "../common/DummySQLSock.h"
#include "../common/strManip.h"

int main() {
	DummySQLSock s("../test-maimutza");

	auto res = s.doQuery("SELECT echipa1,echipa2,data_joc,status_echipe from fotbal");
	while (res->next()) {
		std::cout << res->getString("echipa1") << " : " << res->getString("echipa2") << " [" << res->getInt("status_echipe") << "] "
				<< res->getString("data_joc") << "\n";
	}
}

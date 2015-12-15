#include "../common/listFile.h"
#include <iostream>
#include <string>

int main() {
	auto fn = "../liste/fotbal";
	listFile f = loadListFile(fn);
	saveListFile(fn, f);
}

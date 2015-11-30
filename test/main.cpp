#include "../common/eMailer.h"
#include "../common/configFile.h"
#include "../common/strManip.h"

int main() {
	std::map<std::string, std::string> configOpts;
	parseConfigFile("/home/bog/.ghost.config", configOpts, {"mail-server", "mail-user", "mail-passw", "mail-addr", "mail-dest"});
	std::vector<std::string> dest = strSplit(configOpts["mail-dest"], ',');
    EMailer em(configOpts["mail-server"], configOpts["mail-user"], configOpts["mail-passw"], configOpts["mail-addr"]);

    em.setVerbose(true);
    em.send(dest, "test subject", "Salut bah, uite asta e un mesaj\r\nde test\r\ntrimis de maimuță\r\n");
}

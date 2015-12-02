#include <string.h>

#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

#include "../common/log.h"


static char* target_args[100] { nullptr };
static unsigned nArgs { 0 };


extern char **environ;

char* childPath = 0;

bool spawn(int &ret) {
	pid_t pid;
	LOGLN("spawning target process...")
	char *argv[2+nArgs];
	argv[0] = childPath;
	for (unsigned i=0; i<nArgs; i++)
		argv[i+1] = target_args[i];
	argv[nArgs+1] = nullptr;
	if (posix_spawn(&pid, childPath, NULL, NULL, argv, environ) == 0) {
		LOGLN(childPath << " PID: " << pid);
		LOGLN("going to sleep....");
		waitpid(pid, &ret, 0);
		return true;
	} else {
		LOGLN("posix_spawn error: " << strerror(ret));
		return false;
	}
}

int main(int argc, char* argv[]) {
	LOGPREFIX("watchdog");

	if (argc < 2) {
		ERROR("Need argument (program to spawn)");
		return -1;
	}
	childPath = argv[1];
	for (int i=2; i<argc; i++) {
		target_args[nArgs++] = argv[i];
	}

	int ret = -1;
	do {
		if (spawn(ret)) {
			if (ret != 0) {
				LOGLN("target crashed with code : " << ret);
				LOGLN("restarting process after 5 seconds...");
				sleep(5);
			}
		} else {
			LOGLN("failed to spawn target process. retrying after 5 seconds...");
			sleep(5);
		}
	} while (ret != 0);
	LOGLN("target exited normally. stopping watchdog.");
	return 0;
}

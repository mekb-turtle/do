#include <stdio.h>
#include <unistd.h>
#include <string.h>
void help() {
	fprintf(stderr, "Invalid option, valid ones are\n\
shutdown\n\
restart\n\
suspend\n");
}
void r(char* cmd) {
	// execute cmd, with argv0 = cmd, with no environment variables
	execve(cmd, (char*[]) { cmd }, (char*[]) { NULL });
}
int main(int argc, char* argv[]) {
	if (geteuid() != 0) {
		fprintf(stderr, "No permission\n");
		return 1;
	}
	if (argc != 2) { help(); return 2; }
	else if (strcmp(argv[1], "shutdown") == 0)
		r("/usr/sbin/poweroff");
	else if (strcmp(argv[1], "restart") == 0)
		r("/usr/sbin/reboot");
	else if (strcmp(argv[1], "suspend") == 0)
		r("/usr/sbin/s2disk");
	else { help(); return 2; }
	return 0;
}

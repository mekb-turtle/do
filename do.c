#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#define DO_GROUP "p"
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define strerr (errno == 0 ? "Error" : strerror(errno))
void help() {
	eprintf("Invalid option, valid ones are\n\
shutdown: poweroff\n\
restart: reboot\n\
suspend1: s2disk\n\
suspend2: s2both\n\
");
}
void run(char* cmd) {
	// execute cmd, with argv0 = cmd, with no environment variables
	if (clearenv() != 0) {
		eprintf("clearenv failure\n");
		return;
	}
	execvp(cmd, (char*[]) { cmd, NULL });
	eprintf("execvpe: %s: %s\n", cmd, strerr);
}
bool getgrouplist_(const char *user, gid_t group, gid_t **groups_, int *ngroups_) {
	// getgrouplist but we don't know the size yet
	int cur_ngroups = 0;
	int result = -1;
	int ngroups;
	gid_t *groups = NULL;
	while (result == -1) {
		if (groups) free(groups);
		cur_ngroups += 8;
		groups = calloc(cur_ngroups, sizeof(gid_t));
		ngroups = cur_ngroups;
		result = getgrouplist(user, group, groups, &ngroups);
	}
	if (ngroups < 0) return 0;
	*groups_ = groups;
	*ngroups_ = ngroups;
	return 1;
}
int main(int argc, char* argv[]) {
	if (geteuid() != 0) {
		eprintf("Must run as setuid root\n"); return 1;
		return 1;
	}
	uid_t my_uid = getuid();
	errno = 0;
	struct passwd *my_pwd = getpwuid(my_uid);
	if (errno)   { eprintf("getpwuid: %s\n",        strerr); return errno; }
	if (!my_pwd) { eprintf("Cannot find user %i\n", my_uid); return 1; }
	gid_t *groups;
	int ngroups;
	if (my_uid != 0) {
		if (!getgrouplist_(my_pwd->pw_name, my_pwd->pw_gid, &groups, &ngroups)) {
			eprintf("Failed to get groups of %s\n", my_pwd->pw_name); return 1;
		}
		bool has_group = 0;
		for (int i = 0; i < ngroups; ++i) {
			errno = 0;
			struct group *grp = getgrgid(groups[i]);
			if (errno) { eprintf("getgrgid: %s\n",         strerr); return errno; }
			if (!grp)  { eprintf("Cannot find group %i\n", my_uid); return 1; }
			if (strcmp(grp->gr_name, DO_GROUP) == 0) {
				has_group = 1;
				break;
			}
		}
		free(groups);
		if (!has_group) {
			eprintf("%s is not in group %s\n", my_pwd->pw_name, DO_GROUP);
			return 1;
		}
		errno = 0;
	}
	if (argc != 2) { help(); return 2; }
	else if (strcmp(argv[1], "shutdown") == 0)
		run("/usr/sbin/poweroff");
	else if (strcmp(argv[1], "restart") == 0)
		run("/usr/sbin/reboot");
	else if (strcmp(argv[1], "suspend1") == 0)
		run("/usr/sbin/s2disk");
	else if (strcmp(argv[1], "suspend2") == 0)
		run("/usr/sbin/s2both");
	else { help(); return 2; }
	return 0;
}

/* See LICENSE file for copyright and license details. */
#define _GNU_SOURCE
#define SYSLOG_NAMES
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

static const char *
codename(CODE *code, int c)
{
	for (; code->c_name; ++code) {
		if (code->c_val == c)
			return code->c_name;
	}
	return "unknown";
}

int
main(int argc, char *argv[])
{
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path = _PATH_LOG,
	};
	int s, code;
	FILE *f;
	char *buf = NULL, *pos;
	size_t len = 0;
	ssize_t ret;
	const char *pri, *fac;

	umask(0);
	s = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (s < 0)
		err(1, "socket");
	unlink(addr.sun_path);
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		err(1, "bind");
	f = fdopen(s, "r");
	if (!f)
		err(1, "fdopen");
	setlinebuf(stdout);
	for (;;) {
		ret = getline(&buf, &len, f);
		if (ret == -1)
			break;
		pos = buf;
		if (*pos != '<')
			continue;
		++pos;
		code = strtol(pos, &pos, 10);
		if (*pos != '>')
			continue;
		++pos;
		pri = codename(prioritynames, code & LOG_PRIMASK);
		fac = codename(facilitynames, code & LOG_FACMASK);
		printf("%s.%s: %s", pri, fac, pos);
	}
	fclose(f);

	return 0;
}

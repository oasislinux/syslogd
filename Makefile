.POSIX:

PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin

-include config.mk

CFLAGS+=-Wall -Wpedantic

.PHONY: all
all: syslogd

.PHONY: install
install: syslogd
	mkdir -p $(DESTDIR)$(BINDIR)
	cp syslogd $(DESTDIR)$(BINDIR)

.PHONY: clean
clean:
	rm -f syslogd

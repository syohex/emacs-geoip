EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS)

.PHONY : test

all: geoip-core.so

geoip-core.so: geoip-core.o
	$(LD) -shared $(LDFLAGS) -o $@ $^ -lGeoIP

geoip-core.o: geoip-core.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f geoip-core.so geoip-core.o

test:
	$(EMACS) -Q -batch -L . $(LOADPATH) \
		-l test/test.el \
		-f ert-run-tests-batch-and-exit

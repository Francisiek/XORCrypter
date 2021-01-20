#var
C=gcc
CFLAGS=-static

all : xorc
build : xorc


xorc : xorc.c
	$(C) $(CFLAGS) xorc.c -o xorc

install: xorc
	cp xorc /usr/local/bin

uinstall :
	rm -f xorc.c xorc README Makefile .gitignore

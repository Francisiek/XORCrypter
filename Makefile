#var
C=gcc
CFLAGS=-static

all : xorc

xorc : xorc.c
	$(C) $(CFLAGS) xorc.c -o xorc
	rm -f .gitignore

uinstall :
	rm xorc.c xorc README Makefile

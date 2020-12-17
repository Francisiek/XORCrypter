#var
C=gcc
CFLAGS=-static

all : xorc

xorc : xorc.c
	$(C) $(CFLAGS) xorc.c -o xorc

uinstall :
	rm xorc.c xorc README Makefile

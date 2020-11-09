#var
C=gcc
CFLAGS=-static

all : xorc

xorc : xorc.c
	$(C) $(CFLAGS) xorc.c -o xorc

clean :
	rm xorc.c xorc README Makefile

#var
C=gcc
CFLAGS=-static

all : xorc

xorc : xorc.c
	$(C) $(CFLAGS) xorc.c -o xorc

install: xorc
	cp xorc /usr/local/bin

uninstall :
	rm -f /usr/local/bin/xorc

clean :
	rm -f xorc /usr/local/bin/xorc

realclean : clean
	rm -f xorc.c README Makefile

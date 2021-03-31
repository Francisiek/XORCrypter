#var
CFLAGS=-static
VERSION=1.3
NAME=xorc-$(VERSION)
FILES=$(NAME)/xorc.c $(NAME)/Makefile $(NAME)/README


all : xorc

xorc : xorc.c
	$(CC) $(CFLAGS) xorc.c -o xorc

install: xorc
	cp xorc /usr/local/bin

uninstall :
	rm -f /usr/local/bin/xorc

clean :
	rm -f xorc /usr/local/bin/xorc

realclean : clean
	rm -f xorc.c README Makefile .gitgnore

pack: ../$(NAME).tar.xz ../$(NAME).tar.xz-SHA256-SIGNATURE

#packs all needed files to xorc-$(VERSION).tar.xz
../$(NAME).tar.xz:
	cd ..; ln -s xor $(NAME)
	cd ..; tar cfJv $(NAME).tar.xz $(FILES)
	cd ..; rm $(NAME)

../$(NAME).tar.xz-SHA256-SIGNATURE: ../$(NAME).tar.xz
	cd ..; sha256sum $(NAME).tar.xz > $(NAME).tar.xz-SHA256-SIGNATURE


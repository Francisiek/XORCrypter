
CFLAGS=-static -Wall -std=c99 -g
VERSION=1.4
NAME=xorc-$(VERSION)
DEPEND=src/main.c src/handle_err.c src/mem.c src/xorc.c src/base64/base64.c
FILES=$(NAME)/src $(NAME)/Makefile $(NAME)/README


all : xorc

xorc : xorc.c
	$(CC) $(CFLAGS) $(DEPEND) -o xorc

install : xorc
	cp xorc /usr/local/bin

uninstall :
	rm -f /usr/local/bin/xorc

clean :
	rm -f xorc

realclean : clean
	rm -fr src README Makefile .gitgnore 

pack: ../$(NAME).tar.xz ../$(NAME).tar.xz.SHA256-SIGNATURE

#packs all needed files to xorc-$(VERSION).tar.xz
../$(NAME).tar.xz:
	cd ..; ln -s xor $(NAME)
	cd ..; tar cfJv $(NAME).tar.xz $(FILES)
	cd ..; rm $(NAME)

../$(NAME).tar.xz-SHA256-SIGNATURE: ../$(NAME).tar.xz
	cd ..; sha256sum $(NAME).tar.xz > $(NAME).tar.xz.SHA256-SIGNATURE

packgz: ../$(NAME).tar.gz ../$(NAME).tar.gz.SHA256-SIGNATURE

#packs all needed files to xorc-$(VERSION).tar.gz
../$(NAME).tar.gz:
	cd ..; ln -s xor $(NAME)
	cd ..; tar cfzv $(NAME).tar.gz $(FILES)
	cd ..; rm $(NAME)

../$(NAME).tar.gz-SHA256-SIGNATURE: ../$(NAME).tar.gz
	cd ..; sha256sum $(NAME).tar.gz > $(NAME).tar.gz.SHA256-SIGNATURE


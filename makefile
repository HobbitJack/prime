.POSIX:	
.PHONY:	all clean install release source uninstall
.SUFFIXES:

PREFIX 	?= /usr/local

all:	prime

prime:	prime.c prime.g.c prime.g.h prime.l.c prime.l.h
	c99 -lgmp -o $@ prime.c prime.g.c prime.l.c

prime.l.c prime.l.h:	prime.l
	lex -D_POSIX_C_SOURCE=200809L -o prime.l.c prime.l

prime.g.c prime.g.h:	prime.g
	gengetopt <prime.g
	sed -E 's/(\\n)?[[:blank:]]+\(default=.*\)//' <prime.g.c >prime.g.c.tmp
	mv -f prime.g.c.tmp prime.g.c

clean:
	rm -f prime prime.g.? prime.l.? prime*.tar.gz prime.1.gz Makefile

source:
	rm -f prime_source.tar.gz
	tar -cf prime_source.tar prime.c prime.g prime.l prime.1 makefile
	gzip prime_source.tar

release:	prime
	rm -f prime.tar.gz
	sed 6,33d makefile | sed '2c .PHONY:	install uninstall'> Makefile
	tar -cf prime.tar prime prime.c prime.g prime.l prime.1 Makefile
	gzip prime.tar

install:	prime
	mkdir -p $(PREFIX)/bin/
	install prime $(PREFIX)/bin/
	gzip -k prime.1
	mkdir -p $(PREFIX)/share/man/man1/
	install prime.1.gz $(PREFIX)/share/man/man1/

uninstall:
	rm $(PREFIX)/bin/prime
	rm $(PREFIX)/share/man/man1/prime.1.gz
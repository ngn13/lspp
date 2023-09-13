lspp: main.c
	gcc -o $@ $^

install:
	cp lspp /usr/bin/lspp

.PHONY: install

SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
prefix = /usr

dist/lspp: $(SRCS) $(HEADERS) 
	mkdir -p dist
	gcc -o $@ $(SRCS)

install:
	cp dist/lspp $(DESTDIR)$(prefix)/bin/lspp

uninstall:
	rm $(DESTDIR)$(prefix)/bin/lspp

.PHONY: install

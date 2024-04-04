SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
prefix = /usr
VERSION= 1.2

dist/lspp: $(SRCS) $(HEADERS) 
	mkdir -p dist
	gcc -o $@ -DVERSION=\"${VERSION}\" $(SRCS)

install:
	cp dist/lspp $(DESTDIR)$(prefix)/bin/lspp

uninstall:
	rm $(DESTDIR)$(prefix)/bin/lspp

format:
	clang-format -i -style=file ./*/*.c ./*/*.h

.PHONY: install uninstall format

ifeq ($(wildcard last_build),)
 ifndef gf
  gf = -std=c++11 -g -Wall -fdiagnostics-color=always
 endif
 ifndef go
  go = clang++ -c $(gf)
 endif
else
 ifeq ($(shell cat last_build),debug)
  ifndef gf
   gf = -std=c++11 -g -Wall -fdiagnostics-color=always
  endif
  ifndef go
   go = clang++ -c $(gf)
  endif
 else
  ifndef gf
   gf = -std=c++11 -O3
  endif
  ifndef go
   go = clang++ -c $(gf)
  endif
 endif
endif

SHELL = /bin/bash
.PHONY: auto debug release run gdb clean tar

auto: | last_build
	if [[ $$(cat last_build) == debug ]]; then \
		$(MAKE) debug; \
	else \
		$(MAKE) release; \
	fi

debug: | last_build
	export gf="-std=c++11 -g -Wall -fdiagnostics-color=always"; \
	export go="clang++ -c $$gf"; \
	[[ $$(cat last_build) != debug ]] && $(MAKE) clean; \
	echo debug > last_build; \
	$(MAKE) main

release: | last_build
	export gf="-std=c++11 -O3"; \
	export go="clang++ -c $$gf"; \
	[[ $$(cat last_build) != release ]] && $(MAKE) clean; \
	echo release > last_build; \
	$(MAKE) main

last_build:
	echo debug > last_build

main: \
 main.o \
 mgr.o \
 date.o \
 parser.o
	clang++ $(gf) main.o mgr.o date.o parser.o -o main

main.o: \
 main.cpp \
 parser.h \
 mgr.h \
 cost.h \
 date.h
	$(go) main.cpp -o main.o

mgr.o: \
 mgr.cpp \
 mgr.h \
 cost.h \
 date.h
	$(go) mgr.cpp -o mgr.o

date.o: \
 date.cpp \
 date.h
	$(go) date.cpp -o date.o

parser.o: \
 parser.cpp \
 parser.h \
 mgr.h \
 cost.h \
 date.h
	$(go) parser.cpp -o parser.o

run: $(shell [[ -f last_build ]] && cat last_build || echo debug) | last_build
	./main

gdb: debug
	gdb main

clean:
	rm -f main.o mgr.o date.o parser.o budget.tar.gz main

tar:
	tar -chvz main.cpp parser.h mgr.h cost.h date.h mgr.cpp date.cpp parser.cpp makefile -f budget.tar.gz


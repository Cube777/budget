gf = -std=c++11 -g -Wall -fdiagnostics-color=always
go = g++ -c $(gf)

main: \
 main.o \
 parser.o \
 mgr.o \
 date.o
	g++ $(gf) main.o parser.o mgr.o date.o -o main

main.o: \
 main.cpp \
 parser.h \
 mgr.h \
 cost.h \
 date.h
	$(go) main.cpp -o main.o

parser.o: \
 parser.cpp \
 parser.h \
 mgr.h \
 cost.h \
 date.h
	$(go) parser.cpp -o parser.o

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

run: main
	./main

gdb: main
	gdb main

clean:
	rm -f main.o parser.o mgr.o date.o budget.tar.gz main

tar:
	tar -chvz main.cpp parser.h mgr.h cost.h date.h parser.cpp mgr.cpp date.cpp makefile -f budget.tar.gz

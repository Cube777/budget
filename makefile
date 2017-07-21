main.out : main.cpp
	g++ -static *.cpp -o main.out

run:
	./main.out

clean:
	rm -f main.out

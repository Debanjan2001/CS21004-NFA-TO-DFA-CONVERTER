all: compile out

compile:
	g++ 19CS30014.cpp -o a.out

run:
	./a.out

out:
	./a.out > output.txt

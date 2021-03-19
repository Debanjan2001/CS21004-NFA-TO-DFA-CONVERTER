all: compile run

compile:
	g++ -O2 19CS30014.cpp -o a.out

run:
	./random_test.out > input.txt
	./a.out
out:
	./a.out > output.txt

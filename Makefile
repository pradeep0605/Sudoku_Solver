all: cpp java

cpp:
	g++ SSolver.cpp -o SSolver.out
	g++ interface.cpp -o interface.out

java:
	javac Sudoku.java

clean:
	rm *.out
	rm *.class

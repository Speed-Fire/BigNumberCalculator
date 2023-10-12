# Compilator flags
CFLAGS=-D__LINUX_COMPILATION__ -std=c++17 -c -pedantic -Wall -mn

# Linker flags
LFLAGS=

.PHONY: doc

all: compile doc

compile: Calculator_main.o BigInt.o BigDouble.o Evaluator.o Historizer.o Tester.o
	g++ $(LFLAGS) Calculator_main.o BigInt.o BigDouble.o Evaluator.o Historizer.o Tester.o -o BNCalculator
	
Calculator_main.o: BigNumberCalculator/src/Calculator_main.cpp
	g++ $(CFLAGS) BigNumberCalculator/src/Calculator_main.cpp
	
BigInt.o: BigNumberCalculator/src/BigInt.cpp BigNumberCalculator/hdrs/BigInt.h 
	g++ $(CFLAGS) BigNumberCalculator/src/BigInt.cpp

BigDouble.o: BigNumberCalculator/src/BigDouble.cpp BigNumberCalculator/hdrs/BigDouble.h
	g++ $(CFLAGS) BigNumberCalculator/src/BigDouble.cpp

Evaluator.o: BigNumberCalculator/src/Evaluator.cpp BigNumberCalculator/hdrs/Evaluator.h
	g++ $(CFLAGS) BigNumberCalculator/src/Evaluator.cpp

Historizer.o: BigNumberCalculator/src/Historizer.cpp BigNumberCalculator/hdrs/Historizer.h
	g++ $(CFLAGS) BigNumberCalculator/src/Historizer.cpp
	
Tester.o: BigNumberCalculator/src/Tester.cpp BigNumberCalculator/hdrs/Tester.h
	g++ $(CFLAGS) BigNumberCalculator/src/Tester.cpp

doc:
	doxygen Doxyfile
	
run: BNCalculator
	./BNCalculator

clean:
	rm -rf *o BNCalculator
	rm -r doc/
	mkdir doc
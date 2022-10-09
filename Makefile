CFLAGS = -c -Wall -Werror -Wextra -std=c11 -pedantic
TST_FLAGS = $(shell pkg-config --cflags check)
TST_LIBS = $(shell pkg-config --libs check)
SOURCES = SmartCalc/smart_calc.c
OBJECTS = *.o
OS := $(shell uname -s)
BUILD = build

all: tests

install:
	qmake -o $(BUILD)/Makefile SmartCalc/SmartCalc.pro	
ifeq ($(OS), Darwin)
	-cd $(BUILD); make install
else
	-cd $(BUILD); sudo make install
endif

uninstall:
	rm -rf $(BUILD)

dist:
	qmake -o build/Makefile SmartCalc/SmartCalc.pro
ifeq ($(OS), Darwin)
	cd $(BUILD); make install
else
	cd $(BUILD); sudo make install
endif
	tar -cvf SmartCalc.tar build
	rm -rf build

dvi:
ifeq ($(OS), Darwin)
	open dvi.html
else
	xdg-open dvi.html
endif

tests:
	gcc $(CFLAGS) $(SOURCES) SmartCalc/test.c $(TST_FLAGS)
	gcc $(OBJECTS) $(TST_LIBS)
	-./a.out

gcov_report:
	gcc -fprofile-arcs -ftest-coverage $(SOURCES) SmartCalc/test.c $(TST_LIBS)
	-./a.out
	lcov --no-external -t "test" -o test.info -c -d .
	genhtml -o report test.info
ifeq ($(OS), Darwin)
	open report/index.html
else
	xdg-open report/index.html
endif

leaks:
	gcc $(CFLAGS) $(SOURCES) SmartCalc/test.c $(TST_FLAGS)
	gcc $(OBJECTS) $(TST_LIBS)
	-./SmartCalc/a.out
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./a.out
else
	valgrind --leak-check=full --quiet ./a.out
endif

check:
	cp ../materials/linters/CPPLINT.cfg ./
	-python3 ../materials/linters/cpplint.py --extensions=c $(SOURCES) SmartCalc/test.c
	rm *.cfg
	cppcheck --enable=all --suppress=missingIncludeSystem SmartCalc/*.c

.PHONY: all clean rebuild install uninstall

clean:
	rm -rf *.o *.so *.gch *.gcda *.gcno *.info *.out
	rm -rf report
	rm -rf build-SmartCalc-Desktop-Debug
	rm -rf build-SmartCalc-Desktop_x86_darwin_generic_mach_o_64bit-Debug

rebuild: clean all

# Add some debug flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -fno-omit-frame-pointer -O0 -g3 -fsanitize=address,undefined $(CXXFLAGS)

# A VERY VERY minimalistic makefile

PROJECT_NAME := monopol

employee.o : employee.cpp employee.h
	$(CXX) -c $(CXXFLAGS) employee.cpp -o $@

list_employee.o : list_employee.cpp list_employee.h employee.h
	$(CXX) -c $(CXXFLAGS) list_employee.cpp -o $@

corporation.o : corporation.cpp corporation.h list_employee.h employee.h
	$(CXX) -c $(CXXFLAGS) corporation.cpp -o $@

main.o : main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

test_main.o : test_main.cpp
	$(CXX) -c $(CXXFLAGS) test_main.cpp -o $@

test_employee.o : test_employee.cpp employee.h
	$(CXX) -c $(CXXFLAGS) test_employee.cpp -o $@

test_list_employee.o : test_list_employee.cpp list_employee.h employee.h
	$(CXX) -c $(CXXFLAGS) test_list_employee.cpp -o $@

test_corporation.o : test_corporation.cpp corporation.h list_employee.h employee.h
	$(CXX) -c $(CXXFLAGS) test_corporation.cpp -o $@

$(PROJECT_NAME): employee.o list_employee.o corporation.o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

tests : test_main.o test_employee.o employee.o list_employee.o test_list_employee.o corporation.o test_corporation.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

.DEFAULT_GOAL := all
.PHONY: all clean

all : $(PROJECT_NAME) tests ;

clean:
	-rm *.o $(PROJECT_NAME) tests 


# Add some debug flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -fno-omit-frame-pointer -O0 -g3 -fsanitize=address,undefined $(CXXFLAGS)

# A VERY VERY minimalistic makefile

PROJECT_NAME := monopol

employee.o : employee.cpp employee.h
	$(CXX) -c $(CXXFLAGS) employee.cpp -o $@

list_employee.o : list_employee.cpp list_employee.h employee.h
	$(CXX) -c $(CXXFLAGS) list_employee.cpp -o $@

main.o : main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

$(PROJECT_NAME): employee.o list_employee.o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

.DEFAULT_GOAL := all
.PHONY: all clean

all : $(PROJECT_NAME) ;

clean:
	-rm *.o $(PROJECT_NAME) 

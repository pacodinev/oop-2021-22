#include "doctest.h"

#include "employee.h"

#include <cstring>
#include <iostream>
#include <sstream>

TEST_CASE("construction of employee") 
{
    const char name[] = "Ivan";
    unsigned long long egn = 1234567890ULL;
    const char position[] = "programmer";
    unsigned salary = 100000;

    employee emp(name, egn, position, salary);

    CHECK(std::strcmp(emp.get_name(), name) == 0);
    CHECK(emp.get_name() != name);

    CHECK(std::strcmp(emp.get_position(), position) == 0);
    CHECK(emp.get_position() != position);

    CHECK(emp.get_egn() == egn);
    CHECK(emp.get_salary() == salary);
}

TEST_CASE("setters of employee")
{
    const char name[] = "Ivan", name2[] = "Dragan";
    unsigned long long egn = 1234567890ULL;
    const char position[] = "programmer", position2[] = "QA";
    unsigned salary = 100000, salary2 = 50000;

    employee emp(name, egn, position, salary);

    emp.set_name(name2);
    emp.set_position(position2);
    emp.set_salary(salary2);

    CHECK(std::strcmp(emp.get_name(), name2) == 0);
    CHECK(std::strcmp(emp.get_position(), position2) == 0);
    CHECK(emp.get_egn() == egn);
    CHECK(emp.get_salary() == salary2);

    emp.set_name(name);
    emp.set_position(position);
    emp.set_salary(salary);

    CHECK(std::strcmp(emp.get_name(), name) == 0);
    CHECK(std::strcmp(emp.get_position(), position) == 0);
    CHECK(emp.get_egn() == egn);
    CHECK(emp.get_salary() == salary);
}

TEST_CASE("operator== of employee")
{
    const char name[] = "Ivan", name2[] = "Dragan";
    unsigned long long egn = 1234567890ULL, egn2 = 1023456789ULL;
    const char position[] = "programmer", position2[] = "QA";
    unsigned salary = 100000, salary2 = 50000;

    employee emp(name, egn, position, salary);
    employee emp2(name2, egn2, position2, salary2);
    employee emp3(name, egn, position, salary);

    CHECK(emp == emp3);
    CHECK(emp3 == emp);
    CHECK(!(emp == emp2));
    CHECK(!(emp2 == emp));
}


TEST_CASE("copy constructor of employee")
{
    const char name[] = "Ivan";
    unsigned long long egn = 1234567890ULL;
    const char position[] = "programmer";
    unsigned salary = 100000;

    employee emp(name, egn, position, salary);
    employee emp2(emp);

    CHECK(emp == emp2);
    CHECK(emp2 == emp);
}

TEST_CASE("swap of employee")
{
    const char name[] = "Ivan", name2[] = "Dragan";
    unsigned long long egn = 1234567890ULL, egn2 = 1023456789ULL;
    const char position[] = "programmer", position2[] = "QA";
    unsigned salary = 100000, salary2 = 50000;

    employee emp(name, egn, position, salary);
    employee emp_cpy(emp);
    employee emp2(name2, egn2, position2, salary2);
    employee emp2_cpy(emp2);

    emp.swap(emp2);

    CHECK(emp == emp2_cpy);
    CHECK(emp2 == emp_cpy);

    emp.swap(emp2);

    CHECK(emp == emp_cpy);
    CHECK(emp2 == emp2_cpy);
}

TEST_CASE("copy asigment of employee")
{
    const char name[] = "Ivan", name2[] = "Dragan";
    unsigned long long egn = 1234567890ULL, egn2 = 1023456789ULL;
    const char position[] = "programmer", position2[] = "QA";
    unsigned salary = 100000, salary2 = 50000;

    SUBCASE("from default constructed employee")
    {
        employee emp(name, egn, position, salary);
        employee emp2;
        emp2 = emp;

        CHECK(emp == emp2);
        CHECK(emp2 == emp);
    }

    SUBCASE("from non-default constructed employee")
    {
        employee emp(name, egn, position, salary);
        employee emp2(name2, egn2, position2, salary2);
        emp2 = emp;

        CHECK(emp == emp2);
        CHECK(emp2 == emp);
    }
}

TEST_CASE("test ostream operator<< of employee")
{
    const char name[] = "Ivan";
    unsigned long long egn = 1234567890ULL;
    const char position[] = "programmer";
    unsigned salary = 100000;

    employee emp(name, egn, position, salary);

    std::ostringstream my_cout;

    my_cout << emp;

    CHECK(my_cout.str() == "Name: Ivan\nPosition: programmer\nEGN: 1234567890\nSalary: 100000");
}

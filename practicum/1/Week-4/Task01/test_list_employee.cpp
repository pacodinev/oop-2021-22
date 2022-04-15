#include "doctest.h"

#include "employee.h"
#include "list_employee.h"

#include <cstring>
#include <iostream>
#include <sstream>

static employee generate_employee1()
{
    const char name[] = "Ivan";
    unsigned long long egn = 1234567890ULL;
    const char position[] = "programmer";
    unsigned salary = 100000;

    return employee(name, egn, position, salary);
}

static employee generate_employee2()
{
    const char name2[] = "Dragan";
    unsigned long long egn2 = 1023456789ULL;
    const char position2[] = "QA";
    unsigned salary2 = 50000;

    return employee(name2, egn2, position2, salary2);
}

TEST_CASE("construction of list_employee") 
{
    list_employee le;
    CHECK(le.size() == 0);
}

TEST_CASE("adding to list_employee")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    le.add(emp1);
    CHECK(le.size() == 1);
    le.add(emp2);
    CHECK(le.size() == 2);
}

TEST_CASE("find and get of list_employee")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    CHECK(le.find(emp1) == le.size());

    le.add(emp1);
    std::size_t emp1_idx = le.find(emp1);
    CHECK(emp1_idx != le.size());
    CHECK(le.get(emp1_idx) == emp1);

    CHECK(le.find(emp2) == le.size());
}

TEST_CASE("remove of list_employee")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    le.add(emp1);
    le.add(emp2);
    le.remove(emp2);
    CHECK(le.size() == 1);
    CHECK(le.find(emp2) == le.size());
    le.remove(emp1);
    CHECK(le.size() == 0);
    CHECK(le.find(emp1) == le.size());
}

TEST_CASE("get_avarage_salary")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    le.add(emp1);
    le.add(emp2);

    double avarage_salary = emp1.get_salary() + emp2.get_salary();
    avarage_salary = avarage_salary/2;

    CHECK(le.get_avarage_salary() == doctest::Approx(avarage_salary));
}

TEST_CASE("copy ctor of list_employee")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    le.add(emp1);
    le.add(emp2);

    list_employee le2 (le);

    CHECK(le2.size() == le.size());
    CHECK(le2.get(le.find(emp1)) == emp1);
    CHECK(le2.get(le.find(emp2)) == emp2);
}

TEST_CASE("copy assignment of list_employee")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    list_employee le;

    le.add(emp1);
    le.add(emp2);

    list_employee le2;

    CHECK(le2.size() == 0);

    le2 = le;

    CHECK(le2.size() == le.size());
    CHECK(le2.get(le.find(emp1)) == emp1);
    CHECK(le2.get(le.find(emp2)) == emp2);
}

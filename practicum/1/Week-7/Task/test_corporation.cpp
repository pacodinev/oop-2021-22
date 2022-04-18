#include "corporation.h"
#include "employee.h"

#include "doctest.h"
#include <cstring>

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

TEST_CASE("constructor of corporation")
{
    const char corp_name[] = "BigCorp";
    corporation c(corp_name);

    const corporation &c_cref = c;
    CHECK(std::strcmp(c_cref.get_name(), corp_name)==0);

    CHECK(c_cref.get_employees().size() == 0);
}

TEST_CASE("adding employees")
{
    employee emp1 = generate_employee1();
    employee emp2 = generate_employee2();

    const char corp_name[] = "BigCorp";
    corporation c(corp_name);

    c.get_employees().add(emp1);
    c.get_employees().add(emp2);

    const corporation &c_cref = c;
    CHECK(c_cref.get_employees().find(emp1) != c_cref.get_employees().size());
    CHECK(c_cref.get_employees().find(emp2) != c_cref.get_employees().size());
}

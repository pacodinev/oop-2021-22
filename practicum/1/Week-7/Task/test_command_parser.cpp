#include "doctest.h"

#include "command_parser.h"
#include "corporation.h"
#include "employee.h"
#include "list_employee.h"
#include <cstring>
#include <sstream>

TEST_CASE("set_corporation_name using command_parser")
{
    corporation *corp;
    std::istringstream in;
    in.str("set_corporation_name BigCorp\n"
           "add_employee Ivan 1234567890 QA 1000\n");

    bool res = command_parser(in, nullptr, corp, nullptr, false);

    REQURE(res == true);
    const list_employee &le = corp->get_employees();
    CHECK(le.size() == 1);
    const employee &emp = corp->get_employees().get(0);
    CHECK(std::strcmp(emp.get_name(), "Ivan") == 0);
    CHECK(std::strcmp(emp.get_position(), "QA") == 0);
    CHECK(emp.get_egn() == 1234567890);
    CHECK(emp.get_salary() == 1000*100);
}   

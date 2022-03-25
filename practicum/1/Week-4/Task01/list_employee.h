#ifndef __LIST_EMPLOYEE_H_
#define __LIST_EMPLOYEE_H_

#include "employee.h"

#include <cstddef>

class list_employee
{
private:
    employee *m_arr = nullptr; 
    std::size_t m_size=0, m_capacity=0;

    void allocate_mem(std::size_t req_capacity);

public:
    list_employee() = default;

    list_employee(const list_employee &other);
    list_employee& operator=(const list_employee &other);

    ~list_employee();

    std::size_t size() const;

    employee& get(std::size_t index);
    const employee& get(std::size_t index) const;

    void add(const employee &emp);

    // assumption: egn is unique
    void remove(unsigned long long egn);

    // if egn is not found, return size()
    std::size_t find(unsigned long long egn) const;

    double get_avarage_salary() const;
};

#endif // !__LIST_EMPLOYEE_H_

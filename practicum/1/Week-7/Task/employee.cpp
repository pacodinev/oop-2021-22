#include "employee.h"
#include <new>
#include <cstring>
#include <ostream>
#include <utility>
#include <cassert>
#include <iostream>

employee::employee(const char name[], unsigned long long egn, const char position[], unsigned salary)
{
    char *alloc_name = new (std::nothrow) char [std::strlen(name)+1];
    if(alloc_name == nullptr)
    {
        return;
    }
    char *alloc_position = new (std::nothrow) char [std::strlen(position)+1];
    if(alloc_position == nullptr)
    {
        delete [] alloc_name;
        return;
    }

    m_egn = egn;
    m_salary = salary;
    m_name = alloc_name;
    m_position = alloc_position;
    std::strcpy(m_name, name);
    std::strcpy(m_position, position);
}

employee::employee(const employee &other)
{
    char *alloc_name = new (std::nothrow) char [std::strlen(other.m_name)+1];
    if(alloc_name == nullptr)
    {
        return;
    }
    char *alloc_position = new (std::nothrow) char [std::strlen(other.m_position)+1];
    if(alloc_position == nullptr)
    {
        delete [] alloc_name;
        return;
    }

    m_egn = other.m_egn;
    m_salary = other.m_salary;
    m_name = alloc_name;
    m_position = alloc_position;
    std::strcpy(m_name, other.m_name);
    std::strcpy(m_position, other.m_position);
}

employee& employee::operator= (const employee &other)
{
    employee copy(other);
    swap(copy);
    return *this;
}

void employee::swap(employee &other)
{
    using std::swap;
    swap(m_position, other.m_position);
    swap(m_name, other.m_name);
    swap(m_salary, other.m_salary);
    swap(m_egn, other.m_egn);
}

employee::~employee()
{
    delete [] m_name;
    delete [] m_position;
    // m_name = nullptr;
    // m_position = nullptr;
}

const char* employee::get_name() const
{
    return m_name;
}

void employee::set_name(const char new_name[])
{
    char *alloc_name = new (std::nothrow) char [std::strlen(new_name)+1];
    if(alloc_name == nullptr)
    {
        return;
    }

    delete [] m_name;
    m_name = alloc_name;
    std::strcpy(m_name, new_name);
}

unsigned long long employee::get_egn() const
{
    return m_egn;
}

const char* employee::get_position() const
{
    return m_position;
}

void employee::set_position(const char new_position[])
{
    char *alloc_position = new (std::nothrow) char [std::strlen(new_position)+1];
    if(alloc_position == nullptr)
    {
        return;
    }

    delete [] m_position;
    m_position = alloc_position;
    std::strcpy(m_position, new_position);
}

unsigned employee::get_salary() const
{
    return m_salary;
}

void employee::set_salary(unsigned int new_salary)
{
    m_salary = new_salary;
}

bool employee::operator==(const employee &rhs) const
{
    // lhs === *this
    assert((*this).get_egn() != rhs.get_egn() || 
            ((*this).get_salary() == rhs.get_salary() &&
             std::strcmp((*this).get_name(), rhs.get_name()) == 0 &&
             std::strcmp((*this).get_position(), rhs.get_position()) == 0));
    return (*this).get_egn() == rhs.get_egn();
}

std::ostream& operator<< (std::ostream &out, const employee &emp)
{
    out << "Name: " << emp.get_name() << '\n'
        << "Position: " << emp.get_position() << '\n'
        << "EGN: " << emp.get_egn() << '\n'
        << "Salary: " << emp.get_salary();
    return out;
}

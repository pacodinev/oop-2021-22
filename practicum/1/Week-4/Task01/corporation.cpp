#include "corporation.h"

#include "employee.h"
#include "list_employee.h"
#include <cstring>
#include <new>

corporation::corporation() : m_name(nullptr)
{
}

corporation::corporation(const char name[]) : m_name(nullptr)
{
    m_name = new (std::nothrow) char [std::strlen(name)+1];
    if(m_name == nullptr)
        return; // no memory
    std::strcpy(m_name, name);
}

corporation::corporation(const corporation &other) : m_employees(other.get_employees())
{
    m_name = new (std::nothrow) char [std::strlen(other.get_name())+1];
    if(m_name == nullptr)
    {
        return; // no memory
    }

    std::strcpy(m_name, other.get_name());
}

corporation& corporation::operator=(const corporation &other)
{
    corporation copy(other);
    swap(copy);

    return *this;
}

corporation::~corporation()
{
    delete[] m_name;
}

void corporation::swap(corporation &other)
{
    m_employees.swap(other.m_employees);
    std::swap(m_name, other.m_name);
}

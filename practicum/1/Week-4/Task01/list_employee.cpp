#include "list_employee.h"
#include "employee.h"
#include <cstddef>
#include <algorithm>
#include <cstring>
#include <new>
#include <cassert>

void list_employee::allocate_mem(std::size_t req_capacity)
{
    // TODO: maybe also implement shrinking
    if(m_capacity >= req_capacity)
        return;
    // new_capacity = std::max({2*m_capacity, req_mem, (std::size_t)16}); also works
    std::size_t new_capacity = std::max(2*m_capacity, req_capacity);
    new_capacity = std::max(new_capacity, (std::size_t)16);
    employee *new_arr = new (std::nothrow) employee [new_capacity];
    if(new_arr == nullptr)
    {
        return;
    }
    if(m_arr == nullptr)
    {
        m_arr = new_arr;
        m_capacity = new_capacity;
        return;
    }
    std::copy(m_arr, m_arr+m_size, new_arr);
    delete[] m_arr;
    m_arr = new_arr;
    m_capacity = new_capacity;
}

void list_employee::add(const employee &emp)
{
    std::size_t new_size = m_size + 1;
    allocate_mem(new_size);
    if(m_capacity < m_size)
    {
        // memory allocation failed
        // do nothing
        return; 
    }
    m_size = new_size;
    m_arr[new_size-1] = emp;
}

std::size_t list_employee::size() const
{
    return m_size;
}

employee& list_employee::get(std::size_t index)
{
    assert(index < m_size);
    return m_arr[index];
}

const employee& list_employee::get(std::size_t index) const
{
    assert(index < m_size);
    return m_arr[index];
}

std::size_t list_employee::find(unsigned long long egn) const
{
    for(std::size_t i=0; i<size(); i++)
    {
        if(get(i).get_egn() == egn)
            return i;
    }
    return size();
}

void list_employee::remove(unsigned long long egn)
{
    std::size_t index = find(egn);
    if(index == size())
        return;
    std::size_t last = size()-1;
    get(index).swap(get(last));
    --m_size;
    allocate_mem(m_size);
}

list_employee::list_employee(const list_employee &other)
{
    employee *new_arr = new (std::nothrow) employee[other.size()];
    if(new_arr == nullptr)
    {
        return;
    }
    m_size = other.size();
    m_capacity = other.size();
    m_arr = new_arr;
    std::copy(other.m_arr, other.m_arr+m_size, m_arr);
}

void list_employee::swap(list_employee &other)
{
    std::swap(m_arr, other.m_arr);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

list_employee& list_employee::operator=(const list_employee &other)
{
    list_employee copy(other);
    swap(copy);
    return *this;
}

list_employee::~list_employee()
{
    delete [] m_arr;
}

double list_employee::get_avarage_salary() const
{
    double res = 0.0;
    for(std::size_t i=0; i<m_size; i++)
    {
        res += m_arr[i].get_salary();
    }

    res = res / m_size;

    return res;
}

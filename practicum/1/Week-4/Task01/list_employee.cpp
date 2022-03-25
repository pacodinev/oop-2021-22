#include "list_employee.h"
#include "employee.h"
#include <cstddef>
#include <algorithm>
#include <cstring>
#include <new>

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
    std::copy(m_arr, m_arr+m_size, new_arr);
    delete[] m_arr;
    m_arr = new_arr;
    m_capacity = new_capacity;
}

void list_employee::add(const employee &emp)
{
    std::size_t new_size = m_size + 1;
    allocate_mem(new_size);
    // TODO: check capacity
    if(m_capacity < m_size)
    {
        // memory allocation failed
        // do nothing
        return; 
    }
    m_size = new_size;
    m_arr[new_size-1] = emp;
}

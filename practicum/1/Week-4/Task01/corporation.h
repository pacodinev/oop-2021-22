#include "employee.h"
#include "list_employee.h"

#include <cassert>

class corporation
{
private:
    char *m_name;
    list_employee m_employees;

public:

    corporation();
    explicit corporation(const char name[]);
    corporation(const corporation &other);
    corporation& operator= (const corporation &other);
    ~corporation();

    const list_employee& get_employees() const
    {
        assert(m_name != nullptr);
        return m_employees;
    }
    list_employee& get_employees()
    {
        assert(m_name != nullptr);
        return m_employees;
    }

    void swap (corporation &other);

    const char* get_name() const
    {
        assert(m_name != nullptr);
        return m_name;
    }

};

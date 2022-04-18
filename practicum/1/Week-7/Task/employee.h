#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__

#include <ostream>

class employee
{
private:
    char *m_name = nullptr;
    char *m_position = nullptr;
    unsigned long long m_egn = 0;
    unsigned m_salary = 0; // в стотинки

public:
    employee() = default;
    employee(const char name[], unsigned long long egn, const char position[], unsigned salary);

    ~employee();
    employee(const employee &other);

    employee& operator= (const employee &other);

    bool operator== (const employee &rhs) const;
    bool operator!= (const employee &rhs) const
    {
        return !(*this == rhs);
    }

    void swap(employee &other);

    const char* get_name() const;
    void set_name(const char new_name[]);

    const char* get_position() const;
    void set_position(const char new_position[]);
    
    unsigned long long get_egn() const;

    unsigned get_salary() const;
    void set_salary(unsigned new_salary);
};

std::ostream& operator<< (std::ostream &out, const employee &emp);

#endif // !__EMPLOYEE_H__

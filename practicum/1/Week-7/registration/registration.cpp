#include <cassert>
#include <cstring>
#include <exception>
#include <iostream>
#include <new>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <cstddef>

// only for std::copy, didnt have time
#include <algorithm>

class Date
{
private:
    std::uint8_t _day;
    std::uint8_t _month;
    std::uint16_t _year;

    static bool is_leap(unsigned int year)
    {
        if(year % 400 == 0) return true;
        if(year % 100 == 0) return false;
        if(year % 4 == 0) return true;
        return false;
    }

public:
    /* 
     * конструктор, който създава нов обект за дата с ден day, месец month и година year.
     * Ако датата не е валидна, да се хвърля изключение от тип std::invalid_argument.
     */
    Date(unsigned int day, unsigned int month, unsigned int year)
    {
        if((1 <= month && month <= 12) && (1 <= day && day <= 31) && (year <= UINT16_MAX))
        {
            unsigned days_in_month = 0;
            switch(month)
            {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                days_in_month = 31;
                break;
            case 4: case 6: case 9: case 11:
                days_in_month = 30;
                break;
            case 2:
                days_in_month = (is_leap(year) ? 29 : 28);
                break;
            default: 
                days_in_month = 0;
                break;
            }
            if(day <= days_in_month)
            {
                this->_day = (std::uint8_t)day;
                this->_month = (std::uint8_t)month;
                this->_year = (std::uint16_t)year;
                return;
            }
        }
        throw std::invalid_argument("Date is not valid");
    }

    /*
     * връща деня като unsigned int.
     */
    unsigned int day() const noexcept
    {  return _day;  }

    /*
     * връща месеца като unsigned int.
     */
    unsigned int month() const noexcept
    {  return _month;  }

    /*
     * връща годината като unsigned int.
     */
    unsigned int year() const noexcept
    {  return _year;  }

    /*
     * Връща истина, ако датата съхранена в текущия обект е същата като тази в rhs.
     */
    bool operator==(const Date& rhs) const noexcept
    {
        return (this->_day == rhs._day && this->_month == rhs._month && this->_year == rhs._year);
    }

    /*
     * връща истина, ако датата съхранена в текущия обект е по-ранна от тази съхранена в обекта rhs.
     */
    bool operator<(const Date& rhs) const
    {
        if(this->_year == rhs._year)
        {
            if(this->_month == rhs._month)
            {
                return this->_day < rhs._day;
            }
            return this->_month < rhs._month;
        }
        return this->_year < rhs._year;
    }
};

//-----------------------------------------------------

class Registration
{
public:
    /*
    * регистрационен номер, константа от тип std::string.
    */
    const std::string id;
    /*
     * дата на регистрация, константа от тип Date.
     */
    const Date date;

    Registration() = delete;

    Registration(const std::string& id, const Date& date)
    :id(id), date(date)
    { }

    Registration(const Registration& other) = default;

    Registration& operator= (const Registration& other) = delete;

    ~Registration() = default;

    /*
     * Връща истина, ако номерът и датата на текущия обект съвпадат с тези на rhs.
     */
    bool operator==(const Registration& rhs) const noexcept
    {
        return (this->id == rhs.id && this->date == rhs.date);
    }

    /*
     * Проверява дали текущата регистрация предхожда тази в rhs.
     * Считаме, че една регистрация A предхожда друга регистрация B или (1) ако датата на A
     * е преди тази на B, или (2) ако двете дати съвпадат,
     * но регистрационният номер на A предхожда лексикографски този на B.
     */
    bool operator<(const Registration& rhs) const noexcept
    {
        if(this->date == rhs.date)
        {
            return (this->id < rhs.id);
        }
        return (this->date < rhs.date);
    }
};

//-----------------------------------------------------

class RegistrationList
{
private:

    Registration **m_registrations;
    std::size_t m_size, m_capacity;

public:

    /*
     * създава списък, който може да съдържа най-много _capacity на брой регистрации
     */
    explicit RegistrationList(std::size_t capacity)
        :m_registrations(new Registration* [capacity]()),
         m_size(0), m_capacity(capacity)
    { }

    ~RegistrationList() noexcept
    {
        for(std::size_t i=0; i<m_size; i++)
            delete m_registrations[i];
        delete[] m_registrations;
    }

    RegistrationList(const RegistrationList& other)
        :m_registrations(new Registration* [other.capacity()]()),
         m_size(0), m_capacity(other.capacity())
    {
        std::size_t i;
        for(i=0; i<m_size; i++)
        {
            m_registrations[i] = new (std::nothrow) Registration(other[i]);
            if(m_registrations[i] == nullptr)
                break;
        }
        if(i < m_size)
        {
            // allocation failed
            for(std::size_t j=0; j<=i; j++)
                delete m_registrations[i];
            delete[] m_registrations;
            throw std::bad_alloc();
        }

    }

    void swap(RegistrationList& other)
    {
        std::swap(m_registrations, other.m_registrations);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    RegistrationList& operator=(const RegistrationList& other)
    {
        RegistrationList copy(other);
        swap(copy);
        return *this;
    }


    /*
     * добавя регистрацията с номер id и дата date.
     * Тъй като класът трябва да поддържа регистрациите сортирани в нарастващ ред,
     * тази операция трябва да вмъкне новия запис на подходящо място в списъка.
     * Ако операцията не успее (например няма повече място),
     * да се хвърля изключение от тип std::exception.
     * Операцията да дава strong exception guarantee.
     */
    void insert(const std::string& id, const Date& date)
    {
        if(m_size + 1 > m_capacity)
        {
            throw std::exception();
        }
        Registration *new_reg = new Registration(id, date);
        m_registrations[m_size] = new_reg;
        ++m_size;
        for(std::size_t i=m_size-1; i>0; i--)
        {
            if((*m_registrations[i]) < (*m_registrations[i-1]))
            {
                std::swap(m_registrations[i-1], m_registrations[i]);
            }
        }
    }

    /*
     * достъп до елемента намиращ се на позиция index.
     * Ако такъв няма, да се хвърля изключение std::out_of_range.
     */
    const Registration& at(std::size_t index) const
    {
        if(index >= m_size)
            throw std::out_of_range("No element at index");
        return *m_registrations[index];
    }


    /*
     * достъп до елемента намиращ се на позиция index.
     * Функцията да не прави проверка за коректност дали index е валидна позиция.
     * (В debug режим assert-вайте дали index е валидна позиция).
     */
    const Registration& operator[](std::size_t index) const noexcept
    {
        assert(index < m_size);
        return *m_registrations[index];
    }

    /*
     * Проверява дали списъка е празен (т.е. в него не е била добавена нито една регистрация).
     */
    bool empty() const noexcept
    {
        return size() == 0;
    }

    /*
     * капацитет на списъка.
     */
    std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

    /*
     * брой регистрации добавени в списъка.
     */
    std::size_t size() const noexcept
    {
        return m_size;
    }
};

int main()
{
    std::cout << "Number of registrations: ";
    std::size_t n;
    std::cin >> n;

    RegistrationList rl(n);

    for(std::size_t i=0; i<n; i++)
    {
        while (true)
        {
            try
            {
                std::cout << "For car " << i << ":\n";
                std::string id;
                std::cout << "Registration id: "; std::cin >> id;
                unsigned int day, month, year;
                std::cout << "Registration day: "; std::cin >> day;
                std::cout << "Registration month: "; std::cin >> month;
                std::cout << "Registration year: "; std::cin >> year;
                rl.insert(id, Date(day, month, year));
            }
            catch (std::bad_alloc&)
            {
                throw; // do not handle out of memory conditions, just let it die
            }
            catch (std::exception &e)
            {
                std::cout << "There was an error: " << e.what() << std::endl;
                std::cout << "Please retype the data:" << std::endl;
                continue;
            }

            break;
        }
    }

    std::cout << "Printing the database: \n";

    for(std::size_t i=0; i<rl.size(); i++)
    {
        std::cout << "For car " << i << ":\n";
        std::cout << "Registration id: " << rl[i].id << std::endl;
        std::cout << "Registration day: " << rl[i].date.day() << std::endl;
        std::cout << "Registration month: " << rl[i].date.month() << std::endl;
        std::cout << "Registration year: " << rl[i].date.year() << std::endl;
    }

    return 0;
}

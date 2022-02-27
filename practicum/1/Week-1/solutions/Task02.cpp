#include <algorithm>
#include <iostream>
#include <new>
#include <iomanip>
#include <cstring>
#include <cassert>

constexpr std::size_t STUDENT_NAME_MAX_SIZE = 31;
constexpr std::size_t STUDENT_FN_MAX_SIZE = 15;
constexpr std::size_t MARK_SUBJECT_MAX_SIZE = 7;
constexpr std::size_t STUDENT_MARK_COUNT = 6;

struct subject_mark
{
    unsigned short mark;
    char subject[MARK_SUBJECT_MAX_SIZE+1];
};

struct student
{
    subject_mark marks[STUDENT_MARK_COUNT];
    char name[STUDENT_NAME_MAX_SIZE+1];
    char faculty_number[STUDENT_FN_MAX_SIZE+1];
};

student* students_read(std::size_t &students_size)
{
    std::cin >> students_size;
    if(std::cin.fail())
    {
        students_size = 0;
        return nullptr;
    }

    student *students = new (std::nothrow) student[students_size];
    if(students == nullptr)
    {
        students_size = 0;
        return nullptr;
    }
    
    for(std::size_t i=0; i<students_size; i++)
    {
        std::cin >> std::setw(STUDENT_NAME_MAX_SIZE+1) >> students[i].name 
                 >> std::setw(STUDENT_FN_MAX_SIZE+1) >> students[i].faculty_number;
        for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
        {
            std::cin >> std::setw(MARK_SUBJECT_MAX_SIZE+1) >> students[i].marks[j].subject
                     >> students[i].marks[j].mark;

            if(std::cin.fail() || students[i].marks[j].mark < 2 || students[i].marks[j].mark > 6)
            {
                delete[] students;
                students_size=0;
                return nullptr;
            }
        }
    }

    return students;
}

void students_print(const student students[], std::size_t students_size)
{
    for(std::size_t i=0; i<students_size; i++)
    {
        std::cout << students[i].faculty_number << ' '
                  << students[i].name           << ' ';
        for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
        {
            std::cout << students[i].marks[j].subject << ':' 
                      << students[i].marks[j].mark << ' ';
        }
        std::cout << '\n';
    }
}

const student* student_find_by_name (const student students[], std::size_t students_size, const char name[])
{
    for(std::size_t i=0; i<students_size; i++)
    {
        if(std::strcmp(name, students[i].name) == 0)
            return students+i;
    }
    return nullptr;
}

const student* student_find_by_fn (const student students[], std::size_t students_size, const char faculty_number[])
{
    for(std::size_t i=0; i<students_size; i++)
    {
        if(std::strcmp(faculty_number, students[i].faculty_number) == 0)
            return students+i;
    }
    return nullptr;
}

struct marks_info
{
    // index 0 is 2
    // index 1 is 3
    // and so on
    unsigned short marks_count[5] = {0};
};

marks_info student_get_marks_info (const student &s)
{
    marks_info mi{};
    for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
    {
        unsigned short mark = s.marks[j].mark;
        assert(2 <= mark && mark <= 6);
        mi.marks_count[mark-2]++;
    }

    return mi;
}

char* student_favourite_subjects(const student &s)
{
    unsigned short highest_mark = 0;
    for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
    {
        highest_mark = std::max(highest_mark, s.marks[j].mark);
    }
    assert(2 <= highest_mark && highest_mark <= 6);

    std::size_t string_req_size = 0;
    for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
    {
        if(highest_mark == s.marks[j].mark)
            string_req_size += std::strlen(s.marks[j].subject) + 2; // 2 is for ", "
    }
    assert(string_req_size > 0);
    string_req_size -= 2; // the last has no ", "

    char *favourite_subjects_str = new (std::nothrow) char [string_req_size+1];
    if(favourite_subjects_str == nullptr)
    {
        return nullptr;
    }

    favourite_subjects_str[0] = '\0';
    for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
    {
        if(highest_mark == s.marks[j].mark)
        {
            std::strcat(favourite_subjects_str, s.marks[j].subject);
            string_req_size -= std::strlen(s.marks[j].subject);
            if(string_req_size >= 2)
            {
                std::strcat(favourite_subjects_str, ", ");
                string_req_size -= 2;
            }
        }
    }
    assert(string_req_size == 0);

    return favourite_subjects_str;
}

unsigned student_sum_marks(const student &s)
{
    unsigned sum = 0;
    for(std::size_t j=0; j<STUDENT_MARK_COUNT; j++)
    {
        sum += s.marks[j].mark;
    }
    return sum;
}

int students_cmp_avarage_marks(const student &lhs, const student &rhs)
{
    int sum_marks_lhs = (int)student_sum_marks(lhs);
    int sum_marks_rhs = (int)student_sum_marks(rhs);
    
    // no need to divide, marks are always 6

    if(sum_marks_lhs == sum_marks_rhs)
    {
        return std::strcmp(lhs.faculty_number, rhs.faculty_number); // normal
    }
    return sum_marks_rhs - sum_marks_lhs; // inverted
}

void students_sort_by_marks(student students[], std::size_t students_size)
{
    bool is_sorted = false;
    for(std::size_t i=0; i<students_size && !is_sorted; i++)
    {
        is_sorted = true;
        for(std::size_t j=1; j<students_size-1; j++)
        {
            if(students_cmp_avarage_marks(students[j-1], students[j]) > 0)
            {
                is_sorted = false;
                std::swap(students[j-1], students[j]);
            }
        }
    }
}

int main()
{
    std::cout << "Enter students:\n";

    std::size_t students_size;
    student *students = students_read(students_size);
    if(students == nullptr)
    {
        return 1;
    }

    char name_to_search[STUDENT_NAME_MAX_SIZE+1];
    std::cout << "Search name: ";
    std::cin >> std::setw(STUDENT_NAME_MAX_SIZE+1) >> name_to_search;
    if(std::cin.fail())
    {
        delete[] students;
        return 1;
    }
    const student *student_with_given_name = student_find_by_name(students, students_size, name_to_search);
    if(student_with_given_name == nullptr)
    {
        std::cout << "Could not find student with this name\n";
    }
    else
    {
        std::cout << "Printing information about the student:\n";
        students_print(student_with_given_name, 1);
    }

    char fn_to_search[STUDENT_FN_MAX_SIZE+1];
    std::cout << "Search faculty number: ";
    std::cin >> std::setw(STUDENT_FN_MAX_SIZE+1) >> fn_to_search;
    if(std::cin.fail())
    {
        delete[] students;
        return 1;
    }
    const student *student_with_given_fn = student_find_by_fn(students, students_size, fn_to_search);
    if(student_with_given_fn == nullptr)
    {
        std::cout << "Could not find student with this faculty number\n";
    }
    else
    {
        std::cout << "Printing information about the student:\n";
        students_print(student_with_given_fn, 1);
        std::cout << "Printing mark statistics about the student:\n";
        marks_info mi = student_get_marks_info(*student_with_given_fn);
        for(std::size_t i=2; i<=6; i++)
        {
            std::cout << i << ':' << mi.marks_count[i-2] << ' ';
        }
        std::cout << '\n';

        char *fs = student_favourite_subjects(*student_with_given_fn);
        if(fs != nullptr)
        {
            std::cout << "Favourite subjects of the student are: " << fs << '\n';
            delete[] fs;
        }
    }

    students_sort_by_marks(students, students_size);

    std::cout << "Students sort by marks, are in the following order:\n";
    students_print(students, students_size);

    delete[] students;

    return 0;
}

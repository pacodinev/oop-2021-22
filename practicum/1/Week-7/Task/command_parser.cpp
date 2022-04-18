#include "command_parser.h"

#include <cstring>
#include <ios>
#include <istream>
#include <iomanip>
#include <ostream>
#include <limits>
#include <fstream>
#include "corporation.h"
#include "employee.h"

static bool set_corporation_name (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)out; // silence unused parameter error
    if(corp != nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name already called\n";
        return false;
    }
    // TODO: resizable string
    char name[64];
    in >> std::setw(64) >> name;
    if(in.fail())
    {
        if(err != nullptr)
            (*err) << "set_corporation_name: name read failed\n";
        return false;
    }
    corp = new corporation(name);

    return true;
}

static bool get_corporation_name (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)in;
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    if(out != nullptr)
        (*out) << corp->get_name() << '\n';

    return true;
}

static bool add_employee (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)out;
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    // TODO: resizable string
    char name[64], position[64];
    unsigned long long egn;
    double salary;
    in >> std::setw(64) >> name >> egn >> std::setw(64) >> position >> salary;
    if(in.fail())
    {
        if(err != nullptr)
            (*err) << "add_employee: argument read failed\n";
        return false;
    }
    employee emp(name, egn, position, (unsigned int)salary*100);
    bool res = corp->get_employees().add(emp);

    if(!res)
    {
        if(err != nullptr)
            (*err) << "add_employee: failed to add employee to the corporation, "
                      "probably there is already an employee inside with same EGN\n";
        return false;
    }

    return true;
}

static bool list_employees (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)in;
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    if(out == nullptr)
        return true;

    for(std::size_t i=0; i<corp->get_employees().size(); i++)
    {
        employee &emp = corp->get_employees().get(i);
        (*out) << i << ": " 
               << "EGN: " << emp.get_egn() << ' '
               << "Name: " << emp.get_name() << '\n';
    }

    return true;
}

static bool print_employee (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    unsigned long long egn;
    in >> egn;
    if(in.fail())
    {
        if(err != nullptr)
            (*err) << "print_employee: EGN read failed\n";
        return false;
    }

    std::size_t ind = corp->get_employees().find(egn);
    if(ind == corp->get_employees().size())
    {
        if(err != nullptr)
            (*err) << "print_employee: employee with this EGN is not found\n";
        return false;
    }

    if(out == nullptr)
        return true;

    employee &emp = corp->get_employees().get(ind);
    (*out) << "Name: " << emp.get_name() << ' '
           << "EGN: " << emp.get_egn() << ' '
           << "Position: " << emp.get_position() << ' '
           << "Salary: " << ((double)emp.get_salary()/100.0) << '\n';
    

    return true;
}

static bool remove_employee (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)out;
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    unsigned long long egn;
    in >> egn;
    if(in.fail())
    {
        if(err != nullptr)
            (*err) << "remove_employee: EGN read failed\n";
        return false;
    }

    std::size_t ind = corp->get_employees().find(egn);
    if(ind == corp->get_employees().size())
    {
        if(err != nullptr)
            (*err) << "remove_employee: employee with this EGN is not found\n";
        return false;
    }

    corp->get_employees().remove(egn);

    return true;
}

static bool get_average_salary (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)in;
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }

    if(out != nullptr)
        (*out) << corp->get_employees().get_average_salary() << '\n';

    return true;
}

static bool save_db (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err)
{
    (void)out; // silence unused parameter error
    if(corp == nullptr)
    {
        if(err != nullptr)
            (*err) << "set_corporation_name not called\n";
        return false;
    }
    // TODO: resizable string
    char filename[64];
    in >> std::setw(64) >> filename;
    if(in.fail())
    {
        if(err != nullptr)
            (*err) << "save_db: filename read failed\n";
        return false;
    }

    std::ofstream db_file(filename, std::ofstream::out | std::ofstream::trunc); 
    if(!db_file.is_open())
    {
        if(err != nullptr)
            (*err) << "save_db: could not create/open file\n";
        return false;
    }
    db_file << "set_corporation_name " << corp->get_name() << '\n';
    for(std::size_t i=0; i<corp->get_employees().size(); i++)
    {
        employee &emp = corp->get_employees().get(i);
        db_file << "add_employee " 
                << emp.get_name() << ' ' << emp.get_egn() << ' '
                << emp.get_position() << ' ' << ((double)emp.get_salary()/100) << '\n';
    }
    db_file.flush();

    return true;
}

/*
struct commands_list
{
    const char *command_str;
    bool (*command_func)(std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err);
};
static const commands_list commands [] = 
{
    {"set_corporation_name", set_corporation_name},
    {"get_corporation_name", get_corporation_name},
    // ... 
};
*/

bool command_parser (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err, bool recover_from_errors)
{
    while(!in.eof())
    {
        if(out != nullptr)
            (*out) << ">";
        in.clear();
        char command[32];
        in >> std::setw(32) >> command;
        if(in.eof())
            break;
        if(in.fail())
        {
            if(err != nullptr)
                (*err) << "Command read failed\n";
            if(!recover_from_errors)
                return false;
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore this line
            continue;
        }

        bool command_res;
        if(std::strcmp(command, "set_corporation_name") == 0)
        {
            command_res = set_corporation_name(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "get_corporation_name") == 0)
        {
            command_res = get_corporation_name(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "add_employee") == 0)
        {
            command_res = add_employee(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "list_employees") == 0)
        {
            command_res = list_employees(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "print_employee") == 0)
        {
            command_res = print_employee(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "remove_employee") == 0)
        {
            command_res = remove_employee(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "get_average_salary") == 0)
        {
            command_res = get_average_salary(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        else if(std::strcmp(command, "save_db") == 0)
        {
            command_res = save_db(in, out, corp, err);
            if(!command_res && !recover_from_errors)
                return false;
        }
        /*
        bool ran_command=false;
        for(std::size_t i=0; i<sizeof(commands)/sizeof(commands[0]); i++)
        {
            if(std::strcmp(command, commands[i].command_str) == 0)
            {
                bool command_res = commands[i].command_func(in, out, corp, err);
                ran_command=true;
                if(!command_res && !recover_from_errors)
                    return false;
            }
        }*/
        else if(std::strcmp(command, "exit") == 0)
        {
            return true;
        }
        else
        {
            if(err != nullptr)
                (*err) << "Command not recognized\n";
            if(!recover_from_errors)
                return false;
        }
 
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore this line
    }

    return true;
}

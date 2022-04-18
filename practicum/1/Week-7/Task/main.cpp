#include <fstream>
#include <iostream>

#include "corporation.h"
#include "command_parser.h"

int main(int argc, char *argv[])
{
    corporation *corp = nullptr;

    if(argc == 2)
    {
        std::ifstream db_file(argv[1]);
        // TODO: db_file.is_open
        bool res = command_parser(db_file, nullptr, corp, nullptr, false);
        if(res)
        {
            std::cout << "Successfully loaded " << argv[1] << " database file\n";
        }
        else
        {
            std::cout << "Failed to load " << argv[1] << " database file\n";
            return 1;
        }
    }

    command_parser(std::cin, &std::cout, corp, &std::clog, true);

    delete corp;

    return 0;
}

#include <algorithm>
#include <iostream>
#include <new>
#include <iomanip>
#include <cstring>

struct dictionary_entry
{
    char *search;
    char *replace;
};

void dictionary_delete (const dictionary_entry dict[], std::size_t dict_size)
{
    for(std::size_t i=0; i<dict_size; i++)
    {
        delete[] dict[i].search;
        delete[] dict[i].replace;
    }
    delete[] dict;
}

dictionary_entry* dictionary_read (std::size_t &dict_size)
{
    // With exceptions the checks can be written a lot easier
    std::cin>>dict_size;
    if(std::cin.fail())
    {
        dict_size = 0;
        return nullptr;
    }

    dictionary_entry *dict = new (std::nothrow) dictionary_entry[dict_size];
    if(dict == nullptr)
    {
        dict_size = 0;
        return nullptr;
    }
    
    std::size_t i;
    char *tmp_search=nullptr, *tmp_replace=nullptr;
    for(i=0; i<dict_size; i++)
    {
        // needed if we call dictionary_delete early
        dict[i].search = nullptr;
        dict[i].replace = nullptr;

        std::size_t search_size, replace_size;

        std::cin >> search_size;
        if(std::cin.fail()) break;

        tmp_search = new (std::nothrow) char[search_size+1];
        if(tmp_search == nullptr) break;

        std::cin >> std::setw((int)search_size+1) >> tmp_search;
        if(std::cin.fail()) break;

        std::cin >> replace_size;
        if(std::cin.fail()) break;

        tmp_replace = new (std::nothrow) char[replace_size+1];
        if(tmp_replace == nullptr) break;

        std::cin >> std::setw((int)replace_size+1) >> tmp_replace;
        if(std::cin.fail()) break;

        dict[i].search = tmp_search;
        dict[i].replace = tmp_replace;
        tmp_search = nullptr;
        tmp_replace = nullptr;
    }

    if(i == dict_size)
        return dict;
    
    // if we are here, there was an error
    dictionary_delete(dict, i+1); // only i should also work
    delete[] tmp_search;
    delete[] tmp_replace;
    dict_size = 0;
    return nullptr;
}

char* read_string(std::size_t &string_size)
{
    std::cin >> string_size;
    if(std::cin.fail())
    {
        string_size = 0;
        return nullptr;
    }

    char *str;
    str = new (std::nothrow) char[string_size+1];
    if(str == nullptr)
    {
        string_size = 0;
        return nullptr;
    }
    
    std::cin >> std::ws;
    std::cin.getline(str, (unsigned int)string_size+1);
    if(std::cin.fail())
    {
        delete[] str;
        string_size = 0;
        return nullptr;
    }

    return str;
}

std::size_t calc_new_string_max_size (std::size_t old_str_size)
{
    return std::max(2*old_str_size, (std::size_t)64);
}

const dictionary_entry* dictionary_find_entry (const char *str, const dictionary_entry dict[], std::size_t dict_size)
{
    for(std::size_t i=0; i<dict_size; i++)
    {
        std::size_t entry_size = std::strlen(dict[i].search);
        if(std::strncmp(dict[i].search, str, entry_size) == 0)
        {
            return dict+i;
        }
    }
    return nullptr;
}

void dictionary_replace(char *new_str_beg, char *new_str_end, const char *old_str, const dictionary_entry dict[], std::size_t dict_size)
{
    // assert(new_str_beg < new_str_end);
    while(*old_str != '\0' && new_str_beg != new_str_end)
    {
        const dictionary_entry * sent = dictionary_find_entry(old_str, dict, dict_size);
        if(sent != nullptr)
        {
            std::size_t search_size = std::strlen(sent->search);
            std::size_t replace_size = std::strlen(sent->replace);
            std::size_t str_size_left = (std::size_t)(new_str_end - new_str_beg); // std::distance() probably would be better

            std::strncpy(new_str_beg, sent->replace, str_size_left);
            if(str_size_left < replace_size)
                new_str_beg = new_str_end;
            else
                new_str_beg += replace_size;

            old_str += search_size;
        }
        else
        {
            *(new_str_beg++) = *old_str;
            old_str++;
        }
    }
}

int main()
{
    std::size_t dict_size;
    dictionary_entry *dict;
    dict = dictionary_read(dict_size);
    if(dict == nullptr)
        return 1;

    std::size_t str_size;
    char *str = read_string(str_size);
    if(str == nullptr)
    {
        dictionary_delete(dict, dict_size);
        return 1;
    }

    std::size_t new_str_size = calc_new_string_max_size(str_size);
    char *new_str = new (std::nothrow) char[new_str_size+1];
    if(new_str == nullptr)
    {
        delete[] str;
        dictionary_delete(dict, dict_size);
        return 1;
    }

    dictionary_replace(new_str, new_str+new_str_size, str, dict, dict_size);

    std::cout << new_str << '\n';

    delete[] new_str;
    delete[] str;
    dictionary_delete(dict, dict_size);

    return 0;
}

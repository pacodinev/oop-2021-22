#include "terminal_framebuffer.h"

#include <cstddef>
#include <exception>
#include <stdexcept>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>

#include <iostream>

#else // POSIX system

#include <ncurses.h>

#endif

#ifdef _WIN32

terminal_framebuffer::terminal_size terminal_framebuffer::get_terminal_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    BOOL res = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    if(res == FALSE)
        throw std::runtime_error("STDOUT is not a terminal");
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return {rows, columns};
}

terminal_framebuffer::terminal_framebuffer()
{
    terminal_framebuffer::get_terminal_size cur_size = get_terminal_size();
    fb.resize(cur_size.rows, std::vector<bool>(cur_size.cols, false));
}

void terminal_framebuffer::clear_screen()
{
    clrscr();
}

terminal_framebuffer::~terminal_framebuffer()
{

}

void terminal_framebuffer::render()
{
    clear_screen();

    for(const auto& row : fb)
    {
        for(const auto& pixel : row)
        {
            if(pixel)
                std::cout << FILL_CHAR;
            else
                std::cout << BLANK_CHAR;
        }
    }
    std::cout.flush();

    clear_fb();
}

#else // POSIX system
      
terminal_framebuffer::terminal_size terminal_framebuffer::get_terminal_size()
{
    return {LINES, COLS};
}

terminal_framebuffer::terminal_framebuffer()
{
    initscr();

    terminal_framebuffer::terminal_size cur_size = get_terminal_size();
    fb.resize(cur_size.rows, std::vector<bool>(cur_size.cols, false));
}

terminal_framebuffer::~terminal_framebuffer()
{
    endwin();
}

void terminal_framebuffer::clear_screen()
{
    clear();
}

void terminal_framebuffer::render()
{
    clear_screen();

    for(const auto& row : fb)
    {
        for(const auto& pixel : row)
        {
            if(pixel)
                addch(FILL_CHAR);
            else
                addch(BLANK_CHAR);
        }
    }
    refresh();

    clear_fb();
}

#endif

void terminal_framebuffer::clear_fb()
{
    for(auto& row : fb)
    {
        for(std::size_t i=0; i<row.size(); i++)
        {
            row[i]=false;
        }
    }
}

terminal_framebuffer& terminal_framebuffer::get_instance()
{
    static terminal_framebuffer inst;

    return inst;
}

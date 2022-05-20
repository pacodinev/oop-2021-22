#ifndef __TERMINAL_FRAMEBUFFER_H__
#define __TERMINAL_FRAMEBUFFER_H__

#include <vector>

class terminal_framebuffer
{
private:
    std::vector< std::vector < bool > > fb;

private:
    terminal_framebuffer();


    void clear_screen();
    void clear_fb();

public:
    static constexpr char FILL_CHAR = 'X';
    static constexpr char BLANK_CHAR = ' ';

    terminal_framebuffer(const terminal_framebuffer&) = delete;
    terminal_framebuffer& operator= (const terminal_framebuffer&) = delete;

    ~terminal_framebuffer();

    static terminal_framebuffer& get_instance();

    struct terminal_size
    {
        int rows;
        int cols;
    };

    terminal_size get_terminal_size();

    void set_pixel(int row, int col)
    {
        fb[row][col] = true;
    }
    void clear_pixel(int row, int col)
    {
        fb[row][col] = false;
    }
    void toggle_pixel(int row, int col)
    {
        fb[row][col] = !fb[row][col];
    }

    void render();
    
private:


};

#endif

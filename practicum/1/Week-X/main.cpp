// a basic example program

#include "terminal_framebuffer.h"

#include <chrono>
#include <thread>

#include <iostream>

int main()
{
    terminal_framebuffer &tf = terminal_framebuffer::get_instance();
    terminal_framebuffer::terminal_size ts = tf.get_terminal_size();
    int mindim = std::min(ts.cols, ts.rows);
    std::cout << mindim << std::endl;

    for(int t=0; t<10; t++)
    {

        for(int i=0; i<mindim/2; i++)
        {
            tf.set_pixel(i, i);
        }

        tf.render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for(int i=mindim/2; i<mindim; i++)
        {
            tf.set_pixel(i, i);
        }

        tf.render();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }


    return 0;
}

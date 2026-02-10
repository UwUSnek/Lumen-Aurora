#include "console.hpp"
#include <iostream>


// A mutex that controls the console output.
// This allows multiple threads to write multiple messages at once without being interrupted.
std::mutex console::__internal_consoleLock;




int console::__internal_cout_stream_t::overflow(int c) {
    if(c != EOF) {
        if(c == '\n') std::cout << "\033[K";
        std::cout.put(c); //NOSONAR
    }
    return c;
}
static console::__internal_cout_stream_t __internal_cout_streambuff;
std::ostream console::__internal_cout(&__internal_cout_streambuff);
console::__internal_cout_stream_t_wrapper console::cout;





int console::__internal_cerr_stream_t::overflow(int c) {
    if(c != EOF) {
        if(c == '\n') std::cerr << "\033[K";
        std::cerr.put(c); //NOSONAR
    }
    return c;
}
static console::__internal_cerr_stream_t __internal_cerr_streambuff;
std::ostream console::__internal_cerr(&__internal_cerr_streambuff);
console::__internal_cerr_stream_t_wrapper console::cerr;

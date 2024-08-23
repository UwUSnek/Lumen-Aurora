#include <iostream>
#include <string>


int main(){
    std::string s = "#define macro(a, b) $a + $b\n\nmacro(3, 5)\n#define macro2() test\n";


    std::size_t i = 0;
    std::cout << "START";
    while(i != std::string::npos) {
        i = s.find("define", i);
        std::cout << i << "\n";
        ++i;
    }
}
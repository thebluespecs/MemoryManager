#include <iostream>
#include "MemoryManager.h"

int main(int argc, char const *argv[])
{
    MM::init();

    std::cout << "Free memory = " << MM::freeRemaining() << std::endl;
    int* int_pointer = (int*) MM::allocate(sizeof(int));
    std::cout << "Free memory = " << MM::freeRemaining() << std::endl;
    void* l = MM::allocate(100);
    std::cout << "Free memory = " << MM::freeRemaining() << std::endl;
    *int_pointer = 10000;
    std::cout<<"value of the int : "<<*int_pointer<<std::endl;
    return 0;
}

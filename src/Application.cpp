#include <iostream>

#include "MemoryManager.h"
#include "ThreadPool.h"

extern char MM_pool;


// THREADS
void task1(){

    // while(true) std::cout<<"x"<<std::endl;
    
    std::cout << "Free memory = " << MM::freeRemaining() << std::endl;

    int* int_pointer = new int;

    *int_pointer = 10000;
    std::cout<<"value of the int : "<<*int_pointer<<std::endl;
    delete(int_pointer);

    std::cout << "Free memory = " << MM::freeRemaining() << std::endl;
}
// void task2(){}
// void task3(){}
// void task4(){}


int main(int argc, char const *argv[])
{
    MM::init();
    ThreadPool pool(1);
    auto result = pool.enqueue(task1); 
    return 0;
}

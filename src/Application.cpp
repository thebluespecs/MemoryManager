#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "MemoryManager.h"

extern char MM_pool;
#define TIME_MULTIPLIER 1000000

// THREADS
void* task1(void *unused){    
    std::cout << "Task1: Free memory = " << MM::freeRemaining() << std::endl;
    int* int_pointer = new int;
    *int_pointer = 10000;
    std::cout << "Task1: value of the int = " << *int_pointer << std::endl;
    std::cout << "Task1: Free memory = " << MM::freeRemaining() << std::endl;
    delete(int_pointer);
    std::cout << "Task1: Free memory = " << MM::freeRemaining() << std::endl;
}

void* task2(void *unused){
    usleep(1*TIME_MULTIPLIER);
    std::cout << "Task2: Free memory = " << MM::freeRemaining() << std::endl;
    int* int_pointer = new int;
    *int_pointer = 10000;
    std::cout << "Task2: value of the int = " << *int_pointer << std::endl;
    std::cout << "Task2: Free memory = " << MM::freeRemaining() << std::endl;
    delete(int_pointer);
    std::cout << "Task2: Free memory = " << MM::freeRemaining() << std::endl;
}

int main(int argc, char const *argv[])
{
    MM::init();
    pthread_t thread1, thread2;
    int i1 = pthread_create(&thread1, NULL, task1, NULL);
    int i2 = pthread_create(&thread2, NULL, task2, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);
    return 0;
}

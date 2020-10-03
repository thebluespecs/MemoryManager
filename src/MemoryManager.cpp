#include <cstring>
#include <iostream>
// #include <chrono>
// #include <unordered_map>

#include "MemoryManager.h"

  // allocating 100MB Memory Pool.
const int MM_POOL_SIZE = 1000;
char MM_pool[MM_POOL_SIZE];

namespace MM{

  void init(){
    std::cout<<"*****************************Memory Manager initialised.*****************************"<<std::endl;
    for(size_t i=0; i<MM_POOL_SIZE; i++)
      MM_pool[i]=0x0;
      // std::cout<<"Allocation done"<<std::endl;
  }

  void* allocate(int aSize){
    // Base Cases
    if(aSize > MM_POOL_SIZE){
      onOutOfMemory();
    }

    char* i = MM_pool;
    char firstHeader = 0x0;
    char secondHeader = 0x0;

    char* temp = (char*) &aSize;   //int is of 4 bytes, char is of 1
    firstHeader = 0xC0 | temp[1];   //char pointer points to LSB of int ie. last byte
    secondHeader = temp[0];

    //find right free memory block
    char header_one, header_two;
    int iter_count = 0;
    while(iter_count < MM_POOL_SIZE){
      
      if(*i == 0x0){  // empty block is found
        if(*(i+(aSize-1 + 2)) == 0x0){ //empty block size is enough to store
          break;
        }
        else{  //not enough free sapce, search for another memory block
          i++;
          iter_count++;
          continue;
        }
      }
      header_one = i[0];
      header_two = i[1];
      header_one = 0x3F & header_one;   //remove 11 which is a signature
      int size = 0;
      getSizeInt(&header_one, &header_two, &size);
      i = i+size+2;
      iter_count = iter_count + size+2;
      
    }

    //if there is no free memory
    if(iter_count == MM_POOL_SIZE){
      onOutOfMemory();
    }
    
    if(iter_count+(aSize-1 + 2) >= MM_POOL_SIZE){
      onOutOfMemory();
    }

    i[0] = firstHeader;
    i[1] = secondHeader;
    // returns header + 2 where the data is stored.
    return i+2;
  }

    void deallocate(void* aPointer){
      char *p = (char *)aPointer;
      if (p == 0x0){
        errorLog("Error on trying to deallocate a free memory/null pointer");
      }
      // make the header 0x0
      char *firstHeader = p - 2;
      char *secondHeader = p - 1;
      *firstHeader = 0x3F & *firstHeader;   //remove 11 which is a signature
      int size=0;
      MM::getSizeInt( firstHeader, secondHeader, &size );
      char *temp;
      // make the memory block 0x0
      for( int i=0; i< (size+2); i=i+1 ){
        temp = &MM_pool[i];
        *temp = 0x0;
      }
  }
  //helper function, returns the size of the memory block stored in the 14 bits of the header.
  void getSizeInt(char* firstHeader, char* secondHeader, int *size ){
    char tempSize[4];
    tempSize[3] = 0x0;
    tempSize[2] = 0x0;
    tempSize[1] = *firstHeader;
    tempSize[0] = *secondHeader;
    std::memcpy((void*)size, (void*)tempSize, 4);
  }

  int freeRemaining(){
    char *p = MM_pool;
    int count = 0;
    int iter_count = 0;
    char header_one = 0;
    char header_two = 0;

    while(iter_count < MM_POOL_SIZE){
      if(*p == (char)0x0){
        count++;
        p++;
        iter_count++;
        continue;
      }
      header_one = p[0];
      header_two = p[1];
      header_one = 0x3F & header_one;   //remove 11 which is a signature
      int size = 0;
      getSizeInt(&header_one, &header_two, &size);
      p = p+size+2;
      iter_count = iter_count + size+2;
    }

    return count;
  }

  void onOutOfMemory(){
    std::cerr << "Error Message : Pool out of memory. Deallocate some resources."<<std::endl;;
  }

  // call for any other error condition
  void errorLog(const char* e){
    if ( e == NULL ){
      std::cerr << "Unknown Error" << std::endl;
      exit(1);
    }
    else{
      std::cerr<<e<<std::endl;
      exit(1);
    }
  }

};

// void* operator new(size_t aSize) {    
//   return MM::allocate(aSize); 
// } 
  
// void operator delete(void* aPointer){ 
//   MM::deallocate(aPointer); 
// }
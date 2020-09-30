#include <iostream>
#include "MemoryManager.h"

namespace MM{

  void init(){
    for(size_t i=0; i<MM_POOL_SIZE; i++)
      MM_pool[i]=0x0;
  }
  
};
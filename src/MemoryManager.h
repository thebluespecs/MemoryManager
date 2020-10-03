#pragma once

namespace MM{

  // Initialize any data needed to manage the memory pool
  void init();

  // allocate demanded size and return a pointer inside the memory pool 
  void* allocate(int aSize);

  // Free up a chunk previously allocated
  void deallocate(void* aPointer);
  
  // Helper methods
  // Provided the header, returns the size of the memory block after the header.
  void getSizeInt(char* firstHeader, char* secondHeader, int *size );

  // Will scan the memory pool and return the total free space remaining
  int freeRemaining();

  // Call if no space is left for the allocation request(does not return,
  //  promts to clear resources for further allocations)
  void onOutOfMemory();

  // If the caller makes any illegal request manager should call this 
  // provided failure function (which will not return):
  void errorLog(const char* e);

};


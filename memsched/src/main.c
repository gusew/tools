#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void initAlloc(size_t end, char* memblock) {
  for (size_t i = 0; i < end; ++i)
    memblock[i] = 42;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: %s <mem to allocate [KB]>\n", argv[0]);
    return 1;
  }

  size_t kbytesToAlloc = atoi(argv[1]);
  if (kbytesToAlloc <= 0) {
    printf("Error: Invalid number of KB to allocate: '%s'\n", argv[1]);
    return 2;
  }

/*
  size_t allocSteps = atoi(argv[2]);
  if (allocSteps <= 0) {
    printf("Error: Invalid number of KB in a allocation step: '%s'\n", argv[2]);
    return 2;
  }
  printf("Memory allocation of %lu KB in allocation steps of max. %lu KB.\n", kbytesToAlloc, allocSteps);
*/
  const size_t allocSteps = 1000; // 1 MB [KB]
  const size_t numBlocks = kbytesToAlloc/allocSteps + 1; // number of blocks needed in total

  char** memblock = NULL; // let each block become as large as step size
  memblock = (char**)calloc(numBlocks, sizeof(char*));
  printf("Created an array with %lu blocks (each %lu KB) at address %p.\n", numBlocks, allocSteps, (void*)memblock);

  size_t cntAllocTotal = 0; // KB
  size_t cntAllocNow = 0; // KB
  unsigned blockIdx = 0;
  while (cntAllocTotal < kbytesToAlloc) {
    cntAllocNow = (cntAllocTotal + allocSteps <= kbytesToAlloc ? allocSteps : kbytesToAlloc - cntAllocTotal);
    cntAllocTotal += cntAllocNow;

    memblock[blockIdx] = (char*)calloc(cntAllocNow*1000, sizeof(char));
  
    if (memblock[blockIdx] == NULL) {
      printf("Error: calloc returned NULL.\n");
      break;
    }

    if (errno == ENOMEM) {
      printf("Error: No free memory left to allocate.\n");
      break;
    }
    
    initAlloc(cntAllocNow, memblock[blockIdx]);
    printf("Currently, %lu KB of memory are allocated at address %p (out of %lu KB).\n", cntAllocNow, memblock[blockIdx], cntAllocTotal);
//    sleep(1);

    ++blockIdx;
  }

  printf("Allocated %lu KB of memory which will be freed now...\n", cntAllocTotal);
  
  for (unsigned i = 0; i < blockIdx; ++i)
    free(memblock[i]);
  free(memblock);

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void initAlloc(size_t end, char* memblock) {
  for (size_t i = 0; i < end; ++i)
    memblock[i] = (char)random();
}

/** Do some reading and writing to the block elements. */
void annealBlocks(size_t end, char* mblock1, char* mblock2) {
  for (size_t i = 0; i < end; ++i) {
    if (mblock1[i] > mblock2[i]) {
      --mblock1[i];
      ++mblock2[i];
    }
    else  {
      ++mblock1[i];
      --mblock2[i];
    }
  }
}

/** Calculate and return the sum over all elements in a memory block. */
long int sumOneBlock(size_t end, char* block) {
  long int sum = 0;
  for (size_t i = 0; i < end; ++i) sum += block[i];
  return sum;
}

/** Call ps from a child process in order to show stats on the current memory usage of this process. */
void printStats() {
  // first fork this process
  pid_t procid = fork();

  if (procid == 0) {
    // if child, start execution of ps with relevant arguments
    if (execl("/bin/ps", "/bin/ps", "-C", "memsched", "-o", "cputime,etime,pmem,nice,sched,size,rss,vsz", (char*)NULL) == -1) {
      printf("Error in execl(): %s\n", strerror(errno));
      exit(1); // exit the child process here, as only the parent should proceed with allocating memory
    }
  } 
  else if (procid > 0) { // returned the child's pid to the parent
    // if parent, then wait for a second so that the call to ps can happen "synchronously"
//    sleep(1);
    // and wait for child's termination
    if (waitpid(procid, 0, 0) !=  procid) {
      printf("Error in waitpid(): %s\n", strerror(errno));
    } // else: return to mem allocation loop
  }
  else { // if an error has occured
    printf("Error in fork(): %s\n", strerror(errno));
  }  
}


int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s <mem to allocate [KB]>\n", argv[0]);
    return 1;
  }

  size_t kbytesToAlloc = atoi(argv[1]);
  if (kbytesToAlloc <= 0) {
    printf("Error: Invalid number of KB to allocate: '%s'\n", argv[1]);
    return 2;
  }

  const size_t allocSteps = 100000; // 100 MB [KB]
  const size_t numBlocks = kbytesToAlloc/allocSteps + 1; // number of blocks needed in total

  srandom(kbytesToAlloc);

  char** memblock = NULL; // let each block become as large as step size
  memblock = (char**)calloc(numBlocks, sizeof(char*));
  printf("Created an array with %lu blocks (each %lu KB) at address %p.\n", numBlocks, allocSteps, (void*)memblock);

  size_t cntAllocTotal = 0; // KB
  size_t cntAllocNow = 0; // KB
  unsigned blockIdx = 0;
  while (cntAllocTotal < kbytesToAlloc) {
    cntAllocNow = (cntAllocTotal + allocSteps <= kbytesToAlloc ? allocSteps : kbytesToAlloc - cntAllocTotal);
    cntAllocTotal += cntAllocNow;

    memblock[blockIdx] = (char*)calloc(cntAllocNow*1000, sizeof(char)); // convert from KB to Bytes
  
    if (memblock[blockIdx] == NULL) {
      printf("Error: calloc returned NULL.\n");
      break;
    }

    if (errno == ENOMEM) {
      printf("Error: No free memory left to allocate.\n");
      break;
    }
    
    // initialize all elements of the new block
    //initAlloc(cntAllocNow*1000, memblock[blockIdx]); // with random numbers
    memset(memblock[blockIdx], 1, cntAllocNow*1000); // with constant values

    // anneal two different blocks in order to have some reads and writes
    /*
    for (size_t i = 0, j = blockIdx; i < j; ++i, --j) {
      annealBlocks(cntAllocNow*1000, memblock[i], memblock[j]);
    }
    */

    printf("Currently, %lu KB of memory are allocated at address %p (out of %lu KB).\n", cntAllocNow, memblock[blockIdx], cntAllocTotal);
    printStats();

    ++blockIdx;
  }

  printf("Allocated %lu KB of memory which will be freed now...\n", cntAllocTotal);
  
  // calculate the sum over all allocated blocks
  long int totalBlockSum = 0;
  for (unsigned i = 0; i < blockIdx; ++i) {
    if (i < (blockIdx - 1)) {
      totalBlockSum += sumOneBlock(allocSteps*1000, memblock[i]);
    }
    else {
      totalBlockSum += sumOneBlock(cntAllocNow*1000, memblock[i]);
    }

    free(memblock[i]);
  }
  free(memblock);

  printf("The total sum of all block elements is %ld\n", totalBlockSum);
  return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <random>

const unsigned NEWLINE_AT = 100;

/** 
 * This little file IO tool writes into two different files in an interleaved manner. 
 **/

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <filename1> <filename2>  <numbytes>" << std::endl;
    std::cerr << "\tfilename1 - Path to file A which will be filled with data." << std::endl;
    std::cerr << "\tfilename2 - Path to file B which will be filled with data." << std::endl;
    std::cerr << "\tnumbytes - Number of bytes which will be written to that file." << std::endl;
    return 1;
  }

  std::string filepath1 = argv[1];
  std::string filepath2 = argv[2];
  unsigned numbytes = 0;
  try {
    numbytes = std::stoi(argv[3]);    
  } catch (...) { 
    std::cerr << "Error: Number of bytes is specified in an invalid format (not a number)." << std::endl;
    return 2;
  }


  // first open both files ... 
  std::fstream fhandle1;
  fhandle1.open(filepath1, std::fstream::out | std::fstream::trunc);
  if (!fhandle1.is_open()) {
    std::cerr << "Error: Failed to write to the specified file A (no permissions?)." << std::endl;
    return 3;
  }

  std::fstream fhandle2;
  fhandle2.open(filepath2, std::fstream::out | std::fstream::trunc);
  if (!fhandle2.is_open()) {
    std::cerr << "Error: Failed to write to the specified file B (no permissions?)." << std::endl;
    return 4;
  }

  // ... write in an interleaved pattern to both files ...
  unsigned nextBurstLength = 0; // number of characters to output to one of the files
  unsigned cntFileA = 0; // counts number of bytes already output to each file
  unsigned cntFileB = 0;

  // do some randomization when deciding for the bursts
  std::default_random_engine rndGenerator;
  unsigned fraction = (numbytes > 1000 ? numbytes/100 : 25);
  std::uniform_int_distribution<unsigned> distribution(1, fraction);

  while (cntFileA < numbytes || cntFileB < numbytes) {
    nextBurstLength = distribution(rndGenerator);
    
    if (nextBurstLength % 2 == 0) { // choose file A
      while (cntFileA < numbytes && nextBurstLength > 0) {
        if (cntFileA % (NEWLINE_AT + 1) == NEWLINE_AT) {
          fhandle1 << std::endl;
        } else {
          fhandle1 << "X";
        }
        ++cntFileA;
        --nextBurstLength;
      }
    }
    else {
      while (cntFileB < numbytes && nextBurstLength > 0) {
        if (cntFileB % (NEWLINE_AT + 1) == NEWLINE_AT) {
          fhandle2 << std::endl;
        } else {
          fhandle2 << "X";
        }
        ++cntFileB;
        --nextBurstLength;
      }
    }
  }

  fhandle1.close();
  fhandle2.close();

	return 0;
}


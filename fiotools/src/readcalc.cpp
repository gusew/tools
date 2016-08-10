#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

/**
 * This little file IO tool first reads all bytes from start to end, 
 * then does some processing for some time and afterwards 
 * reads the file in the opposite direction (from end to start). */

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    std::cerr << "\tfilename - Path to file which will be accessed." << std::endl;
    return 1;
  }

  std::string filepath = argv[1];

  // first read file until the end ...
  std::fstream fhandle;

  fhandle.open(filepath, std::fstream::in);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  unsigned haveReadFF = 0;
  unsigned supersum = 0;
  std::string line;

  while (fhandle.good() && fhandle.peek() != EOF) {
    char readchar = fhandle.get();
    ++haveReadFF;
    supersum += readchar;
  }
//  std::cout << "Total number of bytes read: " << std::to_string(haveReadFF) << "." << std::endl;
  //fhandle.close();

  // ... then do some senseless calculations ...
  unsigned divcounter = 0;

  for (unsigned i = 0; i < 999+1/supersum; ++i) {
    for (unsigned j = 0; j < 99+1/supersum; ++j) {
      if ( (i*i/17) % 3 == (j*j/31) % 4) {
        for (unsigned k = i*j; k > 0; --k)
          if (k == i || k == j) divcounter += k*(j + i*13) / (i + j);
      }
    }
  }

  std::cout << "The magic result of the calculation is: " << std::to_string(divcounter) << std::endl;

  
  // ... and the read the file from end to beginning
  unsigned haveReadRE = 0;
  fhandle.seekg(-1, fhandle.cur);

  while (fhandle.good()) {
    char readchar = fhandle.peek();

    if (readchar != EOF) {
      ++haveReadRE;
      supersum -= readchar;
    }

    fhandle.seekg(-1, fhandle.cur);
  }
  fhandle.close();

  if (haveReadFF != haveReadRE) 
    std::cerr << "Number of bytes read in forward and backward direction do not match: " <<
      std::to_string(haveReadFF) << " forward / " << std::to_string(haveReadRE) << " backward." << std::endl;
  
  if (supersum > 0)
    std::cerr << "The supersum value is not zero, but " << std::to_string(supersum) << "." << std::endl;

	return 0;
}


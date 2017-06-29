#include <iostream>
#include <string>
#include "FileHandler.hpp"

void usage(const char* progname) {
  std::cerr << "Usage: " << progname << " (r|k|i) <input file> <output file>" << std::endl;
  std::cerr << "\t<input file> as path to file which shall be read" << std::endl;
  std::cerr << "\t<output file> as path to file which shall be written with results" << std::endl;
  std::cerr << "\tchoose between modes:" << std::endl;
  std::cerr << "\t\t'r' = reduce number of lines by skipping," << std::endl;
  std::cerr << "\t\t'k' = keep number of lines unchanged, or" << std::endl;
  std::cerr << "\t\t'i' = inflate number of lines by repeating"<< std::endl;
}

int main(int argc, char** argv) {

  if (argc < 4) {
    if (argc >= 1)
      usage(argv[0]);
    else
      usage("exampletask");

    return EXIT_FAILURE;
  }

  // analyze input arguments
  bool reduce = false;
  bool inflate = false;

  if (argv[1][0] == 'r')
    reduce = true;
  else if (argv[1][0] == 'i')
    inflate = true;

  std::string infile(argv[2]);
  std::string outfile(argv[3]);

  FileHandler fhin(infile);
  FileHandler fhout(outfile);

  //std::cout << "Example task will start now..." << std::endl;

  fhin.open(true); // open input file for reading only
  fhout.open(false); // open output file for writing

  std::string line;
  unsigned cnt = 0;
  bool writeWasGood = true;

  while (fhin.readLine(line) && writeWasGood) {
    if (reduce && cnt % 2 == 0) {
      writeWasGood = fhout.writeLine(line);
    }
    else if (inflate) {
      writeWasGood = fhout.writeLine(line) & fhout.writeLine(line);
    }
    else if (!reduce && !inflate) {
      writeWasGood = fhout.writeLine(line);
    }
    ++cnt;
  }

  //std::cout << "Example task is done." << std::endl;

  return EXIT_SUCCESS;
}


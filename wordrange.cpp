// Filename: wordrange.cpp
//
// Jessalyn Wang

#include <iostream>
#include "avl.h"
#include <stack>
#include <stdexcept>
#include <fstream>
#include <array>
#include <vector> 
#include <algorithm>
#include <sstream>
#include <cstring>
using namespace std;

int main(int argc, char** argv)
{
  // if there are not enough files given, throw an error
  if (argc != 3)
  {
    throw std::invalid_argument("Usage: ./wordrange <INPUT FILE> <OUTPUT FILE>");
  }
  
  ifstream input; // initialize input for reading
  ofstream output; // initialize output for writing
  
  input.open(argv[1]); // open input
  output.open(argv[2]); // open output 
  
  string command, value; // command is the line read by getline, value is the string to be inserted
  char *com, *val, *op, *second_val; // used for strtok and strdup
  int r; // used for range
  
  
  AVL myAVL; // create an AVL
  
  while(getline(input, command)) // iterate and process all the lines of the input file, one line at a time
  {
    if (command.length() == 0)
      continue;
    com = strdup(command.c_str());
    op = strtok(com, " \t");
    
    val = strtok(NULL, " \t");
    if (val != NULL) // should never happen, but just in case
      second_val = strtok(NULL, " \t");
    
    if (strcmp(op, "i") == 0) { // inserts valyue to avl tree
      value = val;
      myAVL.insert(value);
    }
    if (strcmp(op, "r") == 0) { // finds the range for two given values
      r = myAVL.range(val, second_val);
      output << to_string(r) << endl;
    }
    
    free(com); // free the memory allocated from strdup
  }
  
  myAVL.deleteAVL(); // free the avl tree
  input.close();
  output.close();
  
  return 0;
}

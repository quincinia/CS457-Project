// 
// CS 457 Programming Assignment 1
// Fall 2021
// Jacob Gayban
//
// Basic database management system, supports a few commands
// 

#include <iostream>
#include <sstream>
#include <filesystem>
#include "globals.hpp"
#include "parser.hpp"
#include "classes/Attribute.hpp"

using namespace std;
namespace fs = std::filesystem;
// add "-lstdc++fs" at the end to compile with filesystem
// clang++-7 -pthread -std=c++17 -o main main.cpp -lstdc++fs

// To run a file, use:
// ./main <filename>

// Files can only be read ONCE, at the start of the program, after which
// the program switches to command-line-only mode

int main(int argc, char* argv[]) {
  string test;
  cout << boolalpha;
  
  cout << Attribute("a3", "varchar(20)").toString() << endl;

  // run commands from file (if possible)
  if (argc >= 2) {
    istream* file = new ifstream(argv[1]);
    parseStream(file);
    delete file;
  }
  
  // run commands from command line
  while (parseStream(&cin));
    // getline(cin, test);
}
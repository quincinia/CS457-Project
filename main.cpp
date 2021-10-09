//
// CS 457 Programming Assignment 2
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
#include "classes/Table.hpp"

using namespace std;
namespace fs = std::filesystem;
// add "-lstdc++fs" at the end to compile with filesystem
// clang++-7 -pthread -std=c++17 -o main main.cpp -lstdc++fs

// To run a file, use:
// ./main <filename>

// Files can only be read ONCE, at the start of the program, after which
// the program switches to command-line-only mode

int main(int argc, char *argv[])
{
  string test;
  cout << boolalpha;

  currentDB = "db_1";
  Table table("tbl_1");
  Attribute a("a3", "float");
  Attribute b("a4", "char(20)");
  Attribute c("a5", "varchar(30)");
  table.print();
  table.alter_add("a5", "varchar(30)");
  table.print();
  table.delete_where(Condition(a, ">", "2"));
  table.print();
  string values[] = {"1", "\'haha\'", "\'hehe\'"};
  vector<string> insert(values, values + sizeof(values) / sizeof(values[0]));
  table.insert(insert);
  table.print();
  string attributes[] = {"a3", "a5"};
  vector<string> cols(attributes, attributes + sizeof(attributes) / sizeof(attributes[0]));
  table.select(cols, Condition(b, "=", "\'haha\'")).print();
  table.print();
  vector<pair<string, string> > newVal;
  newVal.push_back(make_pair("a5", "\'newa5\'"));
  table.update(newVal, Condition(c, "=", ""));
  table.print();
  table.update(newVal);
  table.print();

  // run commands from file (if possible)
  if (argc >= 2)
  {
    istream *file = new ifstream(argv[1]);
    parseStream(file);
    delete file;
  }

  // run commands from command line
  while (parseStream(&cin));
  // getline(cin, test);
}
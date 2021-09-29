// 
// CS 457 Programming Assignment 1
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the SELECT command
// 

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef SELECT_HPP
#define SELECT_HPP

// SELECT * FROM table_name;

// can read from column list, but doesn't do anything with it
// SELECT column1, column2, ... FROM table_name;

/**
 * @brief  Prints all contents of file (for * queries)
 * @param  name Name of the table to print
 */
void printFile(string name) {
  ifstream file(name);
  string line;

  // grab the first line
  getline(file, line);

  // if there's nothing in the first few rows, keep grabbing
  while (line.empty() && file.good())
    getline(file, line);

  // print the first row
  cout << line;

  // put the bar before each item otherwise you would have an extra bar at the end
  while (file.good()) {
    getline(file, line);
    if (line.empty()) continue;
    cout << " | " << line;
  }
  cout << endl;
  file.close();
}


/**
 * @brief  If attributes to select are given, parses the attributes list and returns arguments
 * @param  firstArg The first item in the arguments list

 * @return A vector of attributes
 * @note   Will eat the "FROM" from the stream
 */
vector<string> parseSelectList(string firstArg, istream* const line) {
  string word;
  vector<string> args;

  // the first argument got eaten by processSelect, so it needs to be passed again
  if (firstArg.back() == ',') firstArg.pop_back();
  args.push_back(firstArg);

  // process the rest of the arguments list
  *line >> word; // otherwise the FROM will be added to the args list
  while (line->good() && word != "FROM") {
    if (word.back() == ',') word.pop_back();
    args.push_back(word);
    *line >> word;
  }

  return args;
}

/**
 * @brief  Parses and executes the SELECT command
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processSelect(istream* const line) {
  string word;
  *line >> word;
  if (word == "*") {
    // eat the "FROM"
    *line >> word;
    if (word != "FROM") {
      cout << "!Unrecognized command \"" << word << "\". " << "expected 'FROM'.";
    }

    // after "FROM" should be the name of the table
    *line >> word;
    if (word.back() == ';') word.pop_back();
    if (fs::exists(currentDB + "/" + word)) {
      printFile(currentDB + "/" + word);
    } else {
      cout << "!Failed to query table " << word << " because it does not exist." << endl;
    }
  } else {
    vector<string> args = parseSelectList(word, line);
    for (auto s : args)
      cout << s << endl;
    // right now, no select list functionality has been specified, just implementing the ability to parse a list

    // grab the table name
    *line >> word;

    // once we have the arguments and the table name, we can easily query the table
    // using something like "queryTable(table, arguments)"
  }
  return true;
}

#endif
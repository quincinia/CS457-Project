// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the SELECT command
// 

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <vector>
#include "../globals.hpp"
#include "../classes/Table.hpp"
#include "../classes/Condition.h"

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
  cout << line << endl;

  // put the bar before each item otherwise you would have an extra bar at the end
  // obsolete now that the file is row-column
  /*
  while (file.good()) {
    getline(file, line);
    if (line.empty()) continue;
    cout << " | " << line;
  }
  */
  cout << endl;
  file.close();
}


/**
 * @brief  If attributes to select are given, parses the attributes list and returns arguments
 * @param  firstArg The first item in the arguments list
 * @pre    Select list should be 
 * @return A vector of attributes
 * @note   Will eat the "FROM" from the stream
 */
vector<string> parseSelectList(string firstArg, istream* const line) {
  string word;
  vector<string> args;

  // the first argument got eaten by processSelect, so it needs to be passed again
  if (firstArg.back() == ',') firstArg.pop_back();

  // if the string is empty, don't add?
  // eg. item1 , item2
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

// newer version, checks correctness and doesn't eat the FROM
vector<string> readList(istream* const line) {
  string word;
  vector<string> args;

  *line >> word;
  if (word == "*") return args;

  while (line->good() && word.back() == ',') {
    word.pop_back();

    if (resolveWord(word)) {
      throw invalid_argument(string("!Unexpected keyword ") + "\"" + word + "\"");
    }

    if (word.empty()) continue;

    args.push_back(word);
    // cout << word << endl;

    *line >> word;
  }
  
  if (resolveWord(word)) {
    throw invalid_argument(string("!Unexpected keyword ") + "\"" + word + "\"");
  } else {
    args.push_back(word);
    // cout << word << endl;
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
  bool selectAll = false;
  string word;
  vector<string> cols = readList(line);

  // extract the FROM (not checking this rn)
  *line >> word;

  // extract the table name
  *line >> word;

  // title case table name
  word = title_case(word);

  // determine if there is a condition 
  if ((selectAll = (word.back() == ';'))) word.pop_back();

  // if the table doesn't exist, do nothing
  if (!table_exists(word, "query"))
      return false;

  // table exists, print tuples
  Table table(word);
  if (selectAll) {
    table.select(cols).print();
  } else {
    // extract the WHERE (not checking this rn)
    *line >> word;
    table.select(cols, read_condition(line, table)).print();
  }
  return true;
}

#endif
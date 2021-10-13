// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the CREATE command
// 

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef CREATE_HPP
#define CREATE_HPP

// CREATE TABLE table_name (
//     column1 datatype,
//     column2 datatype,
//     column3 datatype,
//     ...
// );

// empty tables
// CREATE TABLE table_name;

// databases
// CREATE DATABASE db_name;

/**
 * @brief  Creates a new table; adds columns if specified
 * @param  name Name of the new table
 * @param  line Stream to get input from
 * @param  args True if there is a columns list
 * 
 * @return True if operation succeeded
 */
bool tableInit(string name, istream* const line, bool args) {
  ofstream table(currentDB + "/" + name);
  if (!args) {
    return true;
  }

  // process arguments (if there are more)
  // accepts an argument list in the form:
  // (name1 type1(, name2 type2)*);

  string col, datatype;

  // grab the opening parentheses
  line->ignore(numeric_limits<streamsize>::max(), '(');

  // this can be changed to a do-while(datatype.back() != ';')
  while (line->good()) {
    *line >> col >> datatype;

    table << col << ' ';

    if (datatype.back() == ',') {
      datatype.pop_back();
      table << datatype << " | ";
      continue;
    }

    if (datatype.back() == ';') {
      datatype.pop_back();
      datatype.pop_back();
      table << datatype;
      break;
    }
  }

  // this might need to be removed
  // table << '\n';

  cout << "Table " << name << " created." << endl;

  return true;
}

/**
 * @brief  Parses and executes the CREATE command
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processCreate(istream* const line) {
  string word;
  
  // grab the qualifier
  *line >> word;

  switch (resolveWord(word)) {
    case DATABASE: {
      // extract db name
      *line >> word;

      if (word.back() == ';') word.pop_back();

      if (fs::exists(word)) {
        cout << "!Failed to create database " << word << " because it already exists." << endl; 
        return false;
      } else {
        // extract this into a separate function if needed
        cout << "Database " << word << " created." << endl;
        fs::create_directory(word);
      }

      break;
    }

    case TABLE: {
      bool args = true;

      // extract table name
      *line >> word;

      // title case table name
      word = title_case(word);

      // semicolon means empty table (no arguments to parse)
      if (word.back() == ';') {
        word.pop_back();
        args = false;
      }

      
      if (!currentDB.empty()) {
        if (fs::exists(currentDB + "/" + word)) {
          // ignore arguments list
          if (args) line->ignore(numeric_limits<streamsize>::max(), ';');
          
          cout << "!Failed to create table " << word << " because it already exists." << endl;
        } else {
          word = title_case(word);
          tableInit(word, line, args);
        }
      } else {
        cout << "!Cannot create table; no database in use." << endl;
      }
      break;
    }

    default: {
      cout << "!Unexpected term: " << word << endl;
      return false;
      break;
    }
  }

  return false;
}

#endif
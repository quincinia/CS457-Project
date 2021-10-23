//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the ALTER command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"
#include "../classes/Table.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef ALTER_HPP
#define ALTER_HPP

// ALTER TABLE table_name
// ADD column_name datatype;

/**
 * @brief  Adds a new column to a table
 * @param  tableName  The table to edit
 * @param  line       Stream to get input from
 * 
 * @return True if operation succeeded (unused)
 * @note   Currently does not check for name conflicts
 */
bool processAdd(string tableName, istream *const line)
{
  string columnName, datatype;

  // grab arguments
  // assuming the statement is correctly formatted
  *line >> columnName >> datatype;
  if (datatype.back() == ';')
    datatype.pop_back();

  // add arguments to table
  Table table(tableName);
  table.alter_add(columnName, datatype);
  table.printFile();
  cout << "Table " << tableName << " modified." << endl;
  return true;
}

/**
 * @brief  Parses and executes the ALTER command
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processAlter(istream *const line)
{
  string word;

  // grab the qualifier
  *line >> word;

  switch (resolveWord(word))
  {
  case TABLE:
  {
    // get the name of the table
    string tableName;
    *line >> tableName;

    // title case table name
    tableName = title_case(tableName);

    // if the table doesn't exist, do nothing
    if (!table_exists(tableName, "alter"))
      return false;

    // grab the verb and handle it accordingly
    *line >> word;
    switch (resolveWord(word))
    {
    case ADD:
    {
      processAdd(tableName, line);
      break;
    }

      // add more cases as program evolves

    default:
    {
      cout << "!Unexpected term: " << word << endl;
      return false;
      break;
    }
    }

    break;
  }

  default:
  {
    cout << "!Unexpected term: " << word << endl;
    return false;
    break;
  }
  }

  return true;
}

#endif
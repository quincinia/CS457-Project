#include <iostream>
#include <sstream>
#include <fstream>
// #include <cstdio>
#include <filesystem>
#include <vector>
#include "globals.hpp"


// This file is now obsolete, functions moved to the "handlers" directory



using namespace std;
namespace fs = std::filesystem;

// handles all actions
// maybe turn this into a folder with each file containing the keyword handler

#ifndef HANDLERS
#define HANDLERS

string currentDB = ""; // see globals.hpp


// ALTER TABLE table_name
// ADD column_name datatype;
bool processAdd(string tableName, stringstream& line) {
  string columnName, datatype;

  // combining these for now; if you need to, break this up
  // into two statements, and check if the stream is good
  // before scanning again

  // if a read fail occurs, return false

  // grab arguments
  line >> columnName >> datatype;
  if (datatype.back() == ';') datatype.pop_back();

  // add arguments to file
  ofstream file(currentDB + "/" + tableName, ios::out | ios::app);
  file << columnName << " " << datatype << endl;
  file.close();
  cout << "Table " << tableName << " modified." << endl;
  return true;
}

bool processAlter  (stringstream& line) {
  string word;
  line >> word;

  if (currentDB.empty()) {
    cout << "!Cannot alter table; no database in use." << endl;
    return false;
  }

  switch (resolveWord(word)) {
    case TABLE: {
      // get the name of the table
      string tableName;
      line >> tableName;

      if (!fs::exists(currentDB + "/" + tableName)) {
        cout << "!Failed to alter table " << tableName << " because it does not exist." << endl;
        return false;
      } 

      // grab the verb and handle it accordingly
      line >> word;
      switch (resolveWord(word)) {
        case ADD: {
          processAdd(tableName, line);
          break;
        }

        default: {
          cout << "!Unexpected term: " << word << endl;
          return false;
          break;
        }
      }

      break;
    }

    default: {
      cout << "!Unexpected term: " << word << endl;
      return false;
      break;
    }
  }

  return true;
}


bool tableInit(string name, stringstream& line) {
  ofstream table(currentDB + "/" + name);
  if (!line.good()) {
    cout << "Table " << name << " created." << endl;
    return true;
  }
  // process arguments (if there are more)
  char inp;
  line.ignore(numeric_limits<streamsize>::max(), '(');
  while (line) {
    line >> inp;
    line >> std::noskipws;

    if (inp == ',') {
      inp = '\n';
      line >> std::skipws;
    }

    if (inp == ')' && line.peek() == ';') {
      break;
    }

    table << inp;
  }
  table << '\n';

  return true;
}

bool processCreate (stringstream& line) {
  string word;
  line >> word;

  switch (resolveWord(word)) {
    case DATABASE: {
      // extract db name
      line >> word;
      // be careful when testing because the semicolon at the end will show up here, not caring about this right now
      if (word.back() == ';') word.pop_back();
      if (fs::exists(word)) {
        cout << "!Failed to create database " << word << "because it already exists." << endl; 
        return false;
      } else {
        fs::create_directory(word);
        cout << "Database " << word << " created." << endl;
        // add more functionality as needed
      }

      break;
    }

    case TABLE: {
      // extract table name
      line >> word;
      if (word.back() == ';') word.pop_back();
      if (!currentDB.empty()) {
        if (fs::exists(currentDB + "/" + word)) {
          cout << "!Failed to create table " << word << "because it already exists." << endl;
        } else {
          tableInit(word, line);
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

bool processDrop   (stringstream& line) {
  string word;
  line >> word;

  switch (resolveWord(word)) {
    case DATABASE: {
      line >> word;
      // semicolons not supported
      if (fs::remove_all(word)) {
        cout << "Database " << word << " deleted." << endl;
      } else {
        cout << "!Failed to delete " << word << " because it does not exist." << endl;
      }
      break;
    }

    case TABLE: {
      line >> word;
      
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


bool processExit   (stringstream& line) {
  cout << "All done." << endl;
  return true;
}


void printFile(string name) {
  ifstream file(name);
  string line;

  // grab the first line
  getline(file, line);

  // if there's nothing in the first few rows, keep grabbing
  while (file.good() && line.empty())
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

// only returns the items in the list, and also eats the "FROM"
vector<string> parseSelectList(string firstArg, stringstream& line) {
  string word;
  vector<string> args;

  // the first argument got eaten by processSelect, so it needs to be passed again
  if (firstArg.back() == ',') firstArg.pop_back();
  args.push_back(firstArg);

  // process the rest of the arguments list
  line >> word; // otherwise the FROM will be added to the args list
  while (line.good() && word != "FROM") {
    if (word.back() == ',') word.pop_back();
    args.push_back(word);
    line >> word;
  }

  return args;
}

bool processSelect (stringstream& line) {
  string word;
  line >> word;
  if (word == "*") {
    // eat the "FROM"
    line >> word;
    if (word != "FROM") {
      cout << "!Unrecognized command \"" << word << "\". " << "expected 'FROM'.";
    }

    // after "FROM" should be the name of the table
    line >> word;
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
  }
  return true;
}


// switch to void?
bool processUse    (stringstream& line) {
  string word;
  line >> word;
  if (word.back() == ';') word.pop_back();

  if (fs::exists(word)) {
    currentDB = word;
    cout << "Using database " << word << endl;
  } else {
    cout << "!" << word << " is not an existing database." << endl;
  }
  
  return true;
}

#endif
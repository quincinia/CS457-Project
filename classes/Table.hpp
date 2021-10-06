// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Defines the Table class, which represents
// a table in the database
// 

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <utility>  // std::pair
#include "../globals.hpp"
#include "Attribute.hpp"
#include "Condition.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef TABLE_HPP
#define TABLE_HPP

class Table {
  private:
    string name;
    
    // find the index of the column (starts at 0)
    int col_num(string name) {
      int i;
      for (i = 0; i < attributes.size(); i++)
        if (attributes[i].getName() == name) break;
      return ((i < attributes.size()) ? i : -1);
    }
    
    bool is_unique(string col_name) {
      return col_num(col_name) == -1;
    }

    // refactor this; does not consider delimiters
    /* string value_at_col(string row, int col) {
      ostringstream item(row);
      string attr;
      for (int i = 0; i <= col; i++)
        item >> attr;
      return attr;
    } */

    // delimited attributes list -> vector of pairs
    vector<pair<string, string>> read_attributes(string line) {
      istringstream list(line);
      string attr, type, delimiter;
      vector<pair<string, string>> attributes;
      while (list.good()) {
        list >> attr >> type >> delimiter;
        attributes.push_back(make_pair(attr, type));
      }
      for (pair<string, string>& p : attributes) {
        cout << "(" << p.first << ", " << p.second << ") ";
      }
      cout << endl;
      return attributes;
    }

    // reads delimited row items
    // requires files to end with a newline
    vector<string> read_delimited_list(string line) {
      istringstream list(line);
      string value;
      vector<string> row;
      char curr;
      while (list.good()) {
        list.get(curr);
        if (curr == '|') {
          value.pop_back();
          row.push_back(value);
          value.clear();
          list.get(curr); // eat the space after the |
        } else {
          value.push_back(curr);
        }
      }
      value.pop_back(); // the last get() will not return anything new, which means the last character read is duplicated
      
      row.push_back(value);
      for (string& col : row) 
        cout << col << '/';
      cout << endl;
      return row;
    }

    // if a row passes the condition, it is kept
    vector<vector<string>> filter_rows(Condition cond) {
      int i = col_num(cond.attribute.getName());
      if (i == -1) {
        return vector<vector<string>>();
      }
      vector<vector<string>> filteredRows;
      for (vector<string> row : rows) {
        if (cond.resolve(row[i])) {
          filteredRows.push_back(row);
        } else {
          continue;
        }
      }
      return filteredRows;
    }

    vector<vector<string>> filter_cols(vector<vector<string>>& unfiltered, vector<Attribute> filters) {
      vector<vector<string>> filtered(unfiltered.size());
      for (Attribute& a : filters) {
        int i = col_num
      }
    }

  public:
    vector<Attribute> attributes;

    // not gonna convert each row into a vector, just leaving it as one big string
    vector<vector<string>> rows;

    Table(string name);


    // obsolete because of select
    void print();
    void printFile();

    void alter_add(string col_name, string datatype);

    void create(vector<pair<string, string>>& cols);

    void delete_where(Condition cond);
    void delete_all();

    void insert(vector<string>& values);

    // instead of overloading, you can have Condition take a sort of "null" value you can check
    Table select(vector<string>& cols);
    Table select(vector<string>& cols, Condition& cond);
    
    void update(vector<pair<string, string>>& cols);
    void update(vector<pair<string, string>>& cols, Condition& cond);
};

Table::Table(string name)
  : name(name) {
    // read file and initialize attributes and rows
    // table correctness will be handled outside of the constructor
    ifstream table(currentDB + "/" + name);
    string line;

    // initialize attributes
    getline(table, line);
    vector<pair<string, string>> schema = read_attributes(line);
    for (pair<string, string>& p : schema) {
      attributes.push_back(Attribute(p.first, p.second));
    }

    while (table.good()) {
      getline(table, line);
      rows.push_back(read_delimited_list(line));
    }
}

void Table::alter_add(string col_name, string datatype) {
  if (is_unique(col_name)) {
    attributes.push_back(Attribute(col_name, datatype));
    // won't write into file 
  }
}

void Table::create(vector<pair<string, string>>& cols) {
  for (pair<string, string>& col : cols) {
    if (is_unique(col.first)) {
      attributes.push_back(Attribute(col.first, col.second));
    } else {
      // throw exception?
    }
  }
}

void Table::delete_where(Condition cond) {
  int i = col_num(cond.attribute.getName());
  if (i == -1) {
    return;
    // or exception?
  }
  vector<vector<string>> newRows;
  for (vector<string> row : rows) {
    if (cond.resolve(row[i])) {
      continue;
    } else {
      newRows.push_back(row);
    }
  }
  rows = newRows;
}

void Table::delete_all() {
  rows.clear();
}

void Table::insert(vector<string>& values) {
  // ignoring type and size checking for now
  rows.push_back(values);
}


#endif 
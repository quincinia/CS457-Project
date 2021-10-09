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
#include <utility> // std::pair
#include "../globals.hpp"
#include "Attribute.hpp"
#include "Condition.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef TABLE_HPP
#define TABLE_HPP

class Table
{
private:
  string name;

  Table(vector<Attribute> attributes, vector<vector<string> > rows)
      : name(""), attributes(attributes), rows(rows)
  {
    // temporary table constructor
    // printFile should be disabled if the name is blank
  }

  // find the index of the column (starts at 0)
  int col_num(string name)
  {
    int i;
    for (i = 0; i < attributes.size(); i++)
      if (attributes[i].getName() == name)
        break;
    return ((i < attributes.size()) ? i : -1);
  }

  bool is_unique(string col_name)
  {
    return col_num(col_name) == -1;
  }

  Attribute query_attributes(string name)
  {
    for (Attribute a : attributes)
    {
      if (a.getName() == name)
        return a;
    }
    return Attribute("", "INVALID_TYPE"); // or throw exception
  }

  // delimited attributes list -> vector of pairs
  vector<pair<string, string> > read_attributes(string line)
  {
    istringstream list(line);
    string attr, type, delimiter;
    vector<pair<string, string> > attributes;
    while (list.good())
    {
      list >> attr >> type >> delimiter;
      attributes.push_back(make_pair(attr, type));
    }
    for (pair<string, string> &p : attributes)
    {
      cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
    return attributes;
  }

  // reads delimited row items
  // don't end files with newline
  // fix the case where the last item in the list is empty (no value)
  vector<string> read_delimited_list(string line)
  {
    istringstream list(line);
    string value;
    vector<string> row;
    char curr;
    while (list.good())
    {
      list.get(curr);
      if (curr == '|')
      {
        value.pop_back();
        row.push_back(value);
        value.clear();
        list.get(curr); // eat the space after the |
      }
      else
      {
        value.push_back(curr);
      }
    }
    value.pop_back(); // the last get() will not return anything new, which means the last character read is duplicated

    row.push_back(value);
    for (string &col : row)
      cout << col << '/';
    cout << endl;
    return row;
  }

  // if a row passes the condition, it is kept
  vector<vector<string> > filter_rows(Condition cond)
  {
    int i = col_num(cond.attribute.getName());
    if (i == -1)
    {
      return vector<vector<string> >();
    }
    vector<vector<string> > filteredRows;
    for (vector<string> row : rows)
    {
      if (cond.resolve(row[i]))
      {
        filteredRows.push_back(row);
      }
      else
      {
        continue;
      }
    }
    return filteredRows;
  }

  // basicaly the opposite of delete_where
  void filter(Condition cond)
  {
    int i = col_num(cond.attribute.getName());
    if (i == -1)
    {
      return;
    }
    vector<vector<string> > filteredRows;
    for (vector<string> row : rows)
    {
      if (cond.resolve(row[i]))
      {
        filteredRows.push_back(row);
      }
      else
      {
        continue;
      }
    }
    rows = filteredRows;
  }

  vector<vector<string> > filter_cols(vector<vector<string> > &unfiltered, vector<Attribute> filters)
  {
    vector<vector<string> > filtered(unfiltered.size());
    for (Attribute &a : filters)
    {
      int j = col_num(a.getName());
      for (int i = 0; i < filtered.size(); i++) {
        filtered[i].push_back(unfiltered[i][j]);
      }
    }

    return filtered;
  }

public:
  vector<Attribute> attributes;

  // not gonna convert each row into a vector, just leaving it as one big string
  vector<vector<string> > rows;

  Table(string name);

  // obsolete because of select
  void print();
  void printFile();

  void alter_add(string col_name, string datatype);

  void create(vector<pair<string, string> > &cols);

  void delete_where(Condition cond);
  void delete_all();

  void insert(vector<string> &values);

  // instead of overloading, you can have Condition take a sort of "null" value you can check
  Table select(vector<string> &cols);
  Table select(vector<string> &cols, Condition cond);

  void update(vector<pair<string, string> > &cols);
  void update(vector<pair<string, string> > &cols, Condition cond);
};

Table::Table(string name)
    : name(name)
{
  // read file and initialize attributes and rows
  // table correctness will be handled outside of the constructor
  ifstream table(currentDB + "/" + name);
  string line;

  // initialize attributes
  getline(table, line);
  vector<pair<string, string> > schema = read_attributes(line);
  for (pair<string, string> &p : schema)
  {
    attributes.push_back(Attribute(p.first, p.second));
  }

  while (table.good())
  {
    getline(table, line);
    rows.push_back(read_delimited_list(line));
  }
}

void Table::print() {
  for (int i = 0; i < attributes.size(); i++) {
    cout << attributes[i].toString();
    if (i+1 < attributes.size()) {
      cout << " | ";
    }
  }
  cout << endl;
  for (vector<string> &row : rows) {
    for (int i = 0; i < row.size(); i++) {
      cout << row[i];
      if (i+1 < row.size()) {
        cout << " | ";
      }
    }
    cout << endl;
  }
}

void Table::alter_add(string col_name, string datatype)
{
  if (is_unique(col_name))
  {
    attributes.push_back(Attribute(col_name, datatype));
    for (vector<string> &row : rows)
      row.push_back("");
    // won't write into file
  }
}

// this should be a constructor, not a function
void Table::create(vector<pair<string, string> > &cols)
{
  for (pair<string, string> &col : cols)
  {
    if (is_unique(col.first))
    {
      attributes.push_back(Attribute(col.first, col.second));
    }
    else
    {
      // throw exception?
    }
  }
}

void Table::delete_where(Condition cond)
{
  int i = col_num(cond.attribute.getName());
  if (i == -1)
  {
    return;
    // or exception?
  }
  vector<vector<string> > newRows;
  for (vector<string> row : rows)
  {
    if (cond.resolve(row[i]))
    {
      continue;
    }
    else
    {
      newRows.push_back(row);
    }
  }
  rows = newRows;
}

void Table::delete_all()
{
  rows.clear();
}

void Table::insert(vector<string> &values)
{
  // ignoring type and size checking for now
  if (values.size() == attributes.size()) {
    // validity checking should also be done within the handler
    rows.push_back(values);
  } else {
    // throw exception?
  }
}

Table Table::select(vector<string> &cols)
{
  vector<Attribute> selectedCols;
  vector<vector<string> > selectedRows;
  for (string col : cols)
  {
    Attribute selectedAttribute = query_attributes(col);
    if (selectedAttribute.getType() == INVALID_TYPE)
    {
      cout << "Type error";
      // throw exception
    }
    selectedCols.push_back(selectedAttribute);
  }
  selectedRows = filter_cols(rows, selectedCols);
  return Table(selectedCols, selectedRows);
}

Table Table::select(vector<string> &cols, Condition cond)
{
  Table newTable(*this);
  newTable.filter(cond);
  return newTable.select(cols);
}

// (col, value)
void Table::update(vector<pair<string, string> > &cols)
{
  for (pair<string, string> &p : cols)
  {
    int i = col_num(p.first);
    if (i != -1)
    {
      for (vector<string> &row : rows)
      {
        row[i] = p.second;
      }
    }
    else
    {
      // throw exception?
      continue;
    }
  }
}

void Table::update(vector<pair<string, string> > &cols, Condition cond)
{
  int cond_col = col_num(cond.attribute.getName());
  for (vector<string> &row : rows)
  {
    if (cond.resolve(row[cond_col]))
    {
      for (pair<string, string> &p : cols)
      {
        int i = col_num(p.first);
        if (i != -1)
        {
          row[i] = p.second;
        }
      }
    }
  }
}
#endif
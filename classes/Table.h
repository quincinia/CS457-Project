//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Declares the Table class, which represents
// a table in the database
//

#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <utility> // std::pair

using namespace std;

class Attribute;
class Condition;

class Table
{
private:
    string name;

    // temporary table constructor, used for SELECTs
    Table(vector<Attribute> attributes, vector<vector<string> > rows);

    // find the index of the column (starts at 0)
    // returns -1 if the column is not found
    int col_num(string name);

    //
    bool is_unique(string col_name);

    // delimited attributes list -> vector of pairs
    vector<pair<string, string> > read_attributes(string line);

    // reads delimited row items
    // don't end files with newline
    vector<string> read_delimited_list(string line);

    // if a row passes the condition, it is kept
    // this function is actually unused, see Table::filter()
    vector<vector<string> > filter_rows(Condition cond);

    // basicaly the opposite of delete_where
    void filter(Condition cond);

    // takes a set of tuples and a set of columns, then removes the attributes
    // from each tuple that don't match what is in the set of columns
    vector<vector<string> > filter_cols(vector<vector<string> > &unfiltered, vector<Attribute> filters);

public:
    vector<Attribute> attributes;

    vector<vector<string> > rows;

    const string alias;

    // read file and initialize attributes and rows
    // table correctness will be handled outside of the constructor
    Table(string name, string alias = "");

    // return the Attribute given its name
    Attribute query_attributes(string name);

    // print the entire table to cout
    void print();

    // print the entire table to file (overwrites existing content)
    void printFile();

    // adds a new attribute to the table
    void alter_add(string col_name, string datatype);

    // this could be a constructor, not a function
    // this is also unused
    void create(vector<pair<string, string> > &cols);

    // deletes tuples that satisfy a condition
    void delete_where(Condition cond);

    // deletes all tuples
    void delete_all();

    // insert a new tuple
    void insert(vector<string> &values);

    // instead of overloading, you can have Condition take a sort of "null" value you can check
    // grab a subset of the original table
    // returns another Table object in case future operations are needed
    Table select(vector<string> &cols);

    // same as above, except only keeps tuples that satisfy a condition
    Table select(vector<string> &cols, Condition cond);

    // changes the of an attribute or set of attributes
    // all tuples are modified
    void update(vector<pair<string, string> > &cols);

    // same as above, except only changes tuples that satisfy a condition
    void update(vector<pair<string, string> > &cols, Condition cond);

    // if louter is true, then performs a left outer join
    // otherwise, performs an inner join
    Table join(Table other, Condition cond, bool louter = false);
};

#endif
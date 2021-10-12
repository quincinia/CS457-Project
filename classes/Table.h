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
#include <utility>  // std::pair

using namespace std;

class Attribute;
class Condition;

class Table
{
private:
    string name;

    Table(vector<Attribute> attributes, vector<vector<string> > rows);

    int col_num(string name);

    bool is_unique(string col_name);

    vector<pair<string, string> > read_attributes(string line);

    vector<string> read_delimited_list(string line);

    vector<vector<string> > filter_rows(Condition cond);

    void filter(Condition cond);

    vector<vector<string> > filter_cols(vector<vector<string> > &unfiltered, vector<Attribute> filters);

public:
    vector<Attribute> attributes;

    vector<vector<string> > rows;

    Table(string name);

    Attribute query_attributes(string name);

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

#endif
//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Defines the Table class, which represents
// a table in the database
//

#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <utility> // std::pair
#include <cstdio>  // sscanf
#include <exception>
#include "../globals.hpp"
#include "Attribute.h"
#include "Condition.h"
#include "Table.h"

using namespace std;

// Private methods
// ---------------

// temporary table constructor, used for SELECTs
Table::Table(vector<Attribute> attributes, vector<vector<string> > rows)
    : name(""), attributes(attributes), rows(rows)
{
    // temporary table constructor
    // printFile should be disabled if the name is blank, but not worrying about that
}

// find the index of the column (starts at 0)
// returns -1 if the column is not found
int Table::col_num(string name)
{
    int i;
    for (i = 0; i < attributes.size(); i++)
        if (attributes[i].getName() == name)
            break;
    return ((i < attributes.size()) ? i : -1);
}

bool Table::is_unique(string col_name)
{
    return col_num(col_name) == -1;
}

// delimited attributes list -> vector of pairs
vector<pair<string, string> > Table::read_attributes(string line)
{
    istringstream list(line);
    string attr, type, delimiter;
    vector<pair<string, string> > attributes;

    // attributes are formatted as:
    // <name1> <type1> | <name2> <type2 ...

    while (list.good())
    {
        list >> attr >> type >> delimiter;
        attributes.push_back(make_pair(attr, type));
    }

    return attributes;
}

// reads delimited row items
// don't end files with newline
vector<string> Table::read_delimited_list(string line)
{
    istringstream list(line);
    string value;
    vector<string> row;
    char curr;

    // build the string character-by-character; this helps accomodate unexpected whitespace within any values
    // we are not expecting whitespace to be within a value (eg. 'space between'), but in the case there is, we can be prepared
    while (list.good())
    {
        list.get(curr);

        // '|' means that the current item is finished, and we can add it to the vector
        if (curr == '|')
        {
            // there is a space before the delimiter which needs to be removed
            value.pop_back();

            // add item
            row.push_back(value);

            // reset item for the next iteration
            value.clear();

            // eat the space after the |
            list.get(curr);
        }
        else
        {
            // otherwise, this item still needs to be read
            value.push_back(curr);
        }
    }
    value.pop_back(); // the last get() will not return anything new, which means the last character read is duplicated

    // add the last item to the vector
    row.push_back(value);

    // for the case where the last two values are null
    while (row.size() < attributes.size())
        row.push_back("");
    return row;
}

// if a row passes the condition, it is kept
// this function is actually unused, see Table::filter()
vector<vector<string> > Table::filter_rows(Condition cond)
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
void Table::filter(Condition cond)
{
    int i = col_num(cond.attribute.getName());
    if (i == -1)
    {
        // if the condition is invalid, then do nothing
        return;
    }
    vector<vector<string> > filteredRows;
    for (vector<string> row : rows)
    {
        if (cond.resolve(row[i]))
        {
            // grab rows that satisfy the condition
            filteredRows.push_back(row);
        }
        else
        {
            continue;
        }
    }
    // throw away all the rows that don't satisfy the condition
    rows = filteredRows;
}

// takes a set of tuples and a set of columns, then removes the attributes
// from each tuple that don't match what is in the set of columns
vector<vector<string> > Table::filter_cols(vector<vector<string> > &unfiltered, vector<Attribute> filters)
{
    // initialize with the number of tuples
    vector<vector<string> > filtered(unfiltered.size());

    // grab only the specified columns
    for (Attribute &a : filters)
    {
        // grab the column refering to that attribute
        int j = col_num(a.getName());

        // process valid columns
        if (j != -1)
        {
            // for each row, add this column
            for (int i = 0; i < filtered.size(); i++)
            {
                // add the value in this column
                filtered[i].push_back(unfiltered[i][j]);
            }
        }
        else
        {
            // if the column doesn't exist, fill it with nothing
            // error handling can be done here, but won't be
            for (int i = 0; i < filtered.size(); i++)
            {
                filtered[i].push_back("");
            }

            // alternatively, this can do nothing, but we aren't really expecting invalid columns
        }
    }

    return filtered;
}

// Public methods
// --------------
Table::Table(string name, string alias)
    : name(name), alias(alias)
{
    // read file and initialize attributes and rows
    // table correctness will be handled outside of the constructor
    ifstream table(currentDB + "/" + name);
    string line;

    // initialize attributes
    getline(table, line);

    // read the schema
    vector<pair<string, string> > schema = read_attributes(line);
    for (pair<string, string> &p : schema)
    {
        attributes.push_back(Attribute(p.first, p.second));
    }

    // read all rows/tuples
    while (table.good())
    {
        getline(table, line);
        rows.push_back(read_delimited_list(line));
    }
}

// return the Attribute given its name
Attribute Table::query_attributes(string name)
{
    for (Attribute a : attributes)
    {
        if (a.getName() == name)
            return a;
    }
    cout << "!\"" << name << "\" is not a recognized attribute." << endl;
    return Attribute("", "INVALID_TYPE"); // or throw exception
}

// print the entire table to cout
void Table::print()
{
    // print schema
    for (int i = 0; i < attributes.size(); i++)
    {
        cout << attributes[i].toString();
        if (i + 1 < attributes.size())
        {
            cout << " | ";
        }
    }
    cout << endl;

    // print the data in each row
    for (vector<string> &row : rows)
    {
        for (int i = 0; i < row.size(); i++)
        {
            cout << row[i];
            if (i + 1 < row.size())
            {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

// print the entire table to file (overwrites existing content)
void Table::printFile()
{
    ofstream file(currentDB + "/" + name);

    // pretty much the same algo as print()
    for (int i = 0; i < attributes.size(); i++)
    {
        file << attributes[i].toString();
        if (i + 1 < attributes.size())
        {
            file << " | ";
        }
    }

    // we don't want an empty newline in our file
    if (rows.size() > 0)
        file << endl;
    for (int i = 0; i < rows.size(); i++)
    {
        for (int j = 0; j < rows[i].size(); j++)
        {
            file << rows[i][j];
            if (j + 1 < rows[i].size())
            {
                file << " | ";
            }
        }
        if (i + 1 < rows.size())
            file << endl;
    }
    file.close();
}

// adds a new attribute to the table
void Table::alter_add(string col_name, string datatype)
{
    // only adds unique attributes
    if (is_unique(col_name))
    {
        // create and add the Attribute
        attributes.push_back(Attribute(col_name, datatype));

        // when a new attribute is added, its value is null for all tuples
        for (vector<string> &row : rows)
            row.push_back("");
    }
    else
    {
        cout << "!Duplicate attribute: " << col_name << " will not be added." << endl;
    }
}

// this could be a constructor, not a function
// this is also unused
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

// deletes tuples that satisfy a condition
void Table::delete_where(Condition cond)
{
    // grab column
    int i = col_num(cond.attribute.getName());
    int num_rows = rows.size();
    if (i == -1)
    {
        cout << "!0 records deleted; unknown column: " << cond.attribute.getName() << endl;
        return;
        // or exception?
    }
    vector<vector<string> > newRows;
    for (vector<string> row : rows)
    {
        // if the item in that column meets the condition, don't keep it
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
    cout << num_rows - newRows.size() << " record" << (num_rows - newRows.size() == 1 ? "" : "s") << " deleted." << endl;
}

// deletes all tuples
void Table::delete_all()
{
    cout << rows.size() << " record" << (rows.size() == 1 ? "" : "s") << " deleted." << endl;
    rows.clear();
}

// insert a new tuple
void Table::insert(vector<string> &values)
{
    // ignoring type and size checking for now
    // add only if the degree matches
    if (values.size() == attributes.size())
    {
        // validity checking should also be done within the handler
        rows.push_back(values);
        cout << "1 new record inserted." << endl;
    }
    else
    {
        // throw exception?
        cout << "!Incorrect number of values, no insertion made." << endl;
    }
}

// grab a subset of the original table
// returns another Table object in case future operations are needed
Table Table::select(vector<string> &cols)
{
    // empty vector means print all (see select.hpp)
    if (cols.size() == 0)
    {
        return *this;
    }
    vector<Attribute> selectedCols;
    vector<vector<string> > selectedRows;

    // grab all the correct attributes
    for (string col : cols)
    {
        Attribute selectedAttribute = query_attributes(col);
        if (selectedAttribute.getType() == INVALID_TYPE)
        {
            cout << "!Type error" << endl;
            // not gonna mess with error handling for now
            // throw exception
        }
        selectedCols.push_back(selectedAttribute);
    }

    // grab all the values corresponding to the chosen attributes
    selectedRows = filter_cols(rows, selectedCols);

    // return a temporary table rather than printing
    return Table(selectedCols, selectedRows);
}

// same as above, except only keeps tuples that satisfy a condition
Table Table::select(vector<string> &cols, Condition cond)
{
    // copy current values
    Table newTable(*this);

    // filter the rows
    newTable.filter(cond);

    // filter the columns
    return newTable.select(cols);
}

// changes the of an attribute or set of attributes
// all tuples are modified
void Table::update(vector<pair<string, string> > &cols)
{
    int num_modified = 0;

    // pairs are (col, value)
    for (pair<string, string> &p : cols)
    {
        int i = col_num(p.first);

        // update only valid columns
        if (i != -1)
        {
            num_modified = rows.size();
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
    cout << num_modified << " record" << (num_modified == 1 ? "" : "s") << " modified." << endl;
}

// same as above, except only changes tuples that satisfy a condition
void Table::update(vector<pair<string, string> > &cols, Condition cond)
{
    int cond_col = col_num(cond.attribute.getName());

    // count the number of tuples modified
    int num_modified = 0;
    for (vector<string> &row : rows)
    {
        // update only correct attribuets and tuples that satisfy the condition
        if (cond_col != -1 && cond.resolve(row[cond_col]))
        {
            num_modified++;

            // pairs take the form: (attribute, value)
            // see update.hpp
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
    cout << num_modified << " record" << (num_modified == 1 ? "" : "s") << " modified." << endl;
}

Table Table::join(Table other, Condition cond, bool louter)
{
    Table copy(*this);

    // merge attributes
    // this can be moved to after the rows
    for (Attribute &a : other.attributes)
    {
        if (!is_unique(a.getName()))
        {
            cout << "!Duplicate attribute detected; proceed with caution" << endl;
            // probably throw an exception
        }
        copy.attributes.push_back(a);
    }

    // pairs are formatted as (this_table, col_num)
    // this_table is true if the attribute belongs to this table
    pair<bool, int> attr1, attr2;

    // determine which table to grab the attribute from, and which column its in
    char alias[20], col[20];
    sscanf(cond.attribute.getName().c_str(), "%[^.].%s", alias, col);
    if (alias == this->alias)
    {
        attr1 = make_pair(true, col_num(col));
    }
    else if (alias == other.alias)
    {
        attr1 = make_pair(false, other.col_num(col));
    }
    else
    {
        // this is taken care of by read_aliased_condition, but putting it here for completeness
        throw invalid_argument("!Unknown alias: " + string(alias));
    }

    // do the same thing for the other attribute
    sscanf(cond.comp.c_str(), "%[^.].%s", alias, col);
    if (alias == this->alias)
    {
        attr2 = make_pair(true, col_num(col));
    }
    else if (alias == other.alias)
    {
        attr2 = make_pair(false, other.col_num(col));
    }
    else
    {
        throw invalid_argument("!Unknown alias: " + string(alias));
    }

    // merge rows
    vector<vector<string> > mergedRows;
    for (vector<string> &row : rows)
    {
        // true if the tuple is included in the output table
        bool isIncluded = false;
        for (vector<string> &otherRow : other.rows)
        {
            vector<string> mergedRow;
            string val1 = (attr1.first ? row : otherRow)[attr1.second];
            string val2 = (attr2.first ? row : otherRow)[attr2.second];
            if (cond.resolve(val1, val2))
            {
                mergedRow.reserve(row.size() + otherRow.size());
                mergedRow.insert(mergedRow.end(), row.begin(), row.end());
                mergedRow.insert(mergedRow.end(), otherRow.begin(), otherRow.end());
                mergedRows.push_back(mergedRow);
                isIncluded = true;
            }
        }

        // if we are doing a left outer join, make sure that all tuples in this table are included
        if (louter && !isIncluded)
        {
            vector<string> louterRow;
            louterRow.reserve(row.size() + other.attributes.size());
            louterRow.insert(louterRow.end(), row.begin(), row.end());
            louterRow.insert(louterRow.end(), other.attributes.size(), "");
            mergedRows.push_back(louterRow);
        }
    }

    copy.rows = mergedRows;

    return copy;
}
#endif
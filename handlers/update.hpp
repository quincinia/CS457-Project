//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the UPDATE command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <utility>
#include <filesystem>
#include "../globals.hpp"
#include "../classes/Condition.h"

using namespace std;
namespace fs = std::filesystem;

#ifndef UPDATE_HPP
#define UPDATE_HPP

vector<pair<string, string> > read_assignments(istream *const line, bool &updateCond)
{
    string col, assignment, value;
    vector<pair<string, string> > args;

    // assumes at least 1 assignment
    do
    {
        // there MUST BE a space on both sides of the assignment
        // eg. item = value, not item=value
        *line >> col >> assignment >> value;
        if (resolveWord(col) || assignment != "=" || resolveWord(value))
        {
            cout << "!Invalid column name or value, or missing assignment" << endl;

            // don't update anything
            return vector<pair<string, string> >();
        }
        else
        {
            // we need a copy because we will be modifying the original value
            string insert = value;

            // get rid of extra characters
            if (insert.back() == ',')
                insert.pop_back();
            if (insert.back() == ';')
            {
                updateCond = false;
                insert.pop_back();
            }

            // pairs take the form: (attribute, value)
            args.push_back(make_pair(col, insert));
        }
    } while (line->good() && value.back() == ',');

    return args;
}

bool processUpdate(istream *const line)
{
    bool updateCond = true;
    string word, tableName;

    // grab table name
    *line >> tableName;

    // title case table name
    tableName = title_case(tableName);

    // if the table doesn't exist, do nothing
    if (!table_exists(tableName, "update"))
        return false;

    // if the locked table is not ours, then ignore the command
    if (is_locked(tableName) && !transaction.owns(tableName))
    {
        cout << "Error: Table " << tableName << " is locked!" << endl;
        transaction.fail();
        line->ignore(numeric_limits<streamsize>::max(), ';');
        return false;
    }

    // table exists, update it
    Table table(tableName);

    // if a transaction exists, use the saved value
    if (transaction.is_active())
    {
        if (transaction.tables.count(tableName))
            table = transaction.tables[tableName];
        transaction.lock(tableName);
    }

    // extract the SET (not checking this rn)
    *line >> word;

    // grab the attributes that will be updated
    vector<pair<string, string> > cols = read_assignments(line, updateCond);

    if (updateCond)
    {
        // extract the WHERE (not checking this rn)
        *line >> word;
        table.update(cols, read_condition(line, table));
    }
    else
    {
        table.update(cols);
    }

    // if a transaction is in progress, then don't save
    if (transaction.is_active())
    {
        // save the updated table back into the transaction
        transaction.tables[tableName] = table;
    }
    else
    {
        table.printFile();
    }

    return true;
}

#endif
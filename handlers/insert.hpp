//
// CS 457 Programming Assignment 3
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the INSERT command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <exception>
#include <filesystem>
#include "../globals.hpp"
#include "../classes/Table.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef INSERT_HPP
#define INSERT_HPP

// currently does not strings with multiple spaces
// eg. 'mobile phone' will not be read correctly
vector<string> read_values(istream *const line)
{
    char value[20];
    char next = ';';
    string insert;
    vector<string> values;

    scanf(" %[^( \n]%c", value, &next);
    insert = value;

    // will not accept arguments if there is a space between the 'S' and the '('
    if (capitalize(insert) == "VALUES" && next == '(')
    {
    }
    else
    {
        // or throw exception
        cout << "!Syntax error, please begin your list with VALUES(" << endl;
        return values;
    }

    do
    {
        scanf(" %[^,;]%c", value, &next);
        // cout << "Value: " << value << ", ";
        // cout << "Next: " << next << endl;
        insert = value;
        values.push_back(insert);
    } while (next != ';');
    values.back().pop_back();

    return values;
}

bool processInsert(istream *const line)
{
    string word;

    // grab next word, expecting INTO
    *line >> word;

    switch (resolveWord(word))
    {
    case INTO:
    {
        // extract table name
        *line >> word;

        // title case table name
        word = title_case(word);

        // if the table doesn't exist, do nothing
        if (!table_exists(word, "insert into"))
            return false;

        // if the locked table is not ours, then ignore the command
        if (is_locked(word) && !transaction.owns(word))
        {
            cout << "Error: Table " << word << " is locked!" << endl;
            transaction.fail();
            line->ignore(numeric_limits<streamsize>::max(), ';');
            return false;
        }

        // table exists, insert
        vector<string> values = read_values(line);

        Table table(word);

        // if a transaction exists, use the saved value
        if (transaction.is_active())
        {
            if (transaction.tables.count(word))
                table = transaction.tables[word];
            transaction.lock(word);
        }

        table.insert(values);

        // if a transaction is in progress, then don't save
        if (transaction.is_active())
        {
            // save the updated table back into the transaction
            transaction.tables[word] = table;
        }
        else
        {
            table.printFile();
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
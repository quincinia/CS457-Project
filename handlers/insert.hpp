//
// CS 457 Programming Assignment 2
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

        // table exists, insert
        vector<string> values = read_values(line);
        Table table(word);
        table.insert(values);
        table.printFile();
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
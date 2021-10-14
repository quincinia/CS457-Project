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
vector<string> read_values(istream* const line) {
    string value;
    vector<string> values;

    *line >> value;
    if (capitalize(value.substr(0, 7)) == "VALUES(") {
        value.erase(0, 7);
    } else {
        throw invalid_argument("!Syntax error, please begin your list with VALUES(");
    }

    while (line->good()) {
        if (value.back() == ',') {
            value.pop_back();
            values.push_back(value);
            continue;
        }

        if (value.back() == ';') {
            value.pop_back();
            value.pop_back();
            values.push_back(value);
            break;
        }

        *line >> value;
    }

    return values;
}

bool processInsert(istream* const line) {
    string word;

    // grab next word, expecting INTO
    *line >> word;

    switch(resolveWord(word)) {
        case INTO: {
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
            cout << "1 new record inserted." << endl;

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

#endif 
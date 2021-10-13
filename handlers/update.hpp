// 
// CS 457 Programming Assignment 2
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

vector<pair<string, string>> read_assignments(istream* const line, bool &updateCond) {
    string col, assignment, value;
    vector<pair<string, string>> args;

    // assumes at least 1 assignment
    do {
        *line >> col >> assignment >> value;
        if (resolveWord(col) || assignment != "=" || resolveWord(value))
            throw invalid_argument("!Invalid column name or value, or missing assignment");
        else {
            string insert = value;
            if (insert.back() == ',') insert.pop_back();
            if (insert.back() == ';') {
                updateCond = false;
                insert.pop_back();
            }
            args.push_back(make_pair(col, insert));
        }
    } while (line->good() && value.back() == ',');

    return args;
}

bool processUpdate(istream* const line) {
    bool updateCond = true;
    string word;

    // grab table name
    *line >> word;

    // title case table name
    word = title_case(word);

    if (!currentDB.empty()) {
        if (fs::exists(currentDB + "/" + word)) {
            Table table(word);
            // extract the SET (not checking this rn)
            *line >> word;
            vector<pair<string, string> > cols = read_assignments(line, updateCond);
            if (updateCond) {
                // extract the WHERE (not checking this rn)
                *line >> word;
                table.update(cols, read_condition(line, table));
            } else {
                table.update(cols);
            }
            table.printFile();
        } else {
            cout << "!Failed to update table " << word << " because it does not exist." << endl;
            return false;
        }
    } else {
        cout << "!Failed to update table; no database in use." << endl;
        return false;
    }
    return true;
}

#endif 
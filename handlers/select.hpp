//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the SELECT command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <vector>
#include "../globals.hpp"
#include "../classes/Table.hpp"
#include "../classes/Condition.h"
#include "joins.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef SELECT_HPP
#define SELECT_HPP

// SELECT * FROM table_name;

// can read from column list, but doesn't do anything with it
// SELECT column1, column2, ... FROM table_name;

/**
 * @brief  Prints all contents of file (for * queries)
 * @param  name Name of the table to print
 */
void printFile(string name)
{
    ifstream file(name);
    string line;

    // grab the first line
    getline(file, line);

    // if there's nothing in the first few rows, keep grabbing
    while (line.empty() && file.good())
        getline(file, line);

    // print the first row
    cout << line << endl;

    // put the bar before each item otherwise you would have an extra bar at the end
    // obsolete now that the file is row-column
    /*
  while (file.good()) {
    getline(file, line);
    if (line.empty()) continue;
    cout << " | " << line;
  }
  */

    cout << endl;
    file.close();
}

/**
 * @brief  If attributes to select are given, parses the attributes list and returns arguments
 * @param  firstArg The first item in the arguments list
 * @pre    Select list should be 
 * @return A vector of attributes
 * @note   Will eat the "FROM" from the stream
 * @note   Obsolete due to readList()
 */
vector<string> parseSelectList(string firstArg, istream *const line)
{
    string word;
    vector<string> args;

    // the first argument got eaten by processSelect, so it needs to be passed again
    if (firstArg.back() == ',')
        firstArg.pop_back();

    // if the string is empty, don't add?
    // eg. item1 , item2
    args.push_back(firstArg);

    // process the rest of the arguments list
    *line >> word; // otherwise the FROM will be added to the args list
    while (line->good() && word != "FROM")
    {
        if (word.back() == ',')
            word.pop_back();
        args.push_back(word);
        *line >> word;
    }

    return args;
}

// newer version, checks correctness and doesn't eat the FROM
vector<string> readList(istream *const line)
{
    string word;
    vector<string> args;

    *line >> word;
    // empty arguments means select all
    if (word == "*")
        return args;

    while (line->good() && word.back() == ',')
    {
        word.pop_back();

        // check if the item is a keyword
        if (resolveWord(word))
        {
            cout << "!Unexpected keyword \"" + word + "\"" << endl;
            return vector<string>();
        }

        if (word.empty())
            continue;

        args.push_back(word);
        // cout << word << endl;

        *line >> word;
    }

    // the last item in the list needs to be processed manually
    if (resolveWord(word))
    {
        cout << "!Unexpected keyword \"" + word + "\"" << endl;
        return vector<string>();
    }
    else
    {
        args.push_back(word);
        // cout << word << endl;
    }

    // return all attributes in the list
    return args;
}

/**
 * @brief  Parses and executes the SELECT command
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processSelect(istream *const line)
{
    bool selectAll = false;
    string word;

    // read SELECT attributes list
    vector<string> cols = readList(line);

    // extract the FROM (not checking this rn)
    *line >> word;

    string tbl1, alias1;
    string tbl2, alias2;

    try
    {
        switch (read_FROM_clause(line, tbl1, alias1, tbl2, alias2))
        {
        case 1:
        case 2:
            singular_join(line, tbl1, cols);
            return true;
            break;
        case 3:
        case 4:
            inner_join(line, tbl1, alias1, tbl2, alias2, cols);
            return true;
            break;
        case 5:
            left_outer_join(line, tbl1, alias1, tbl2, alias2, cols);
            return true;
            break;
        default:
            return false;
            break;
        }
    }
    catch (const exception &e)
    {
        cout << e.what() << endl;
        return false;
    }
    return false;
}

#endif
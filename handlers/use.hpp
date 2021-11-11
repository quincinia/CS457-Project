//
// CS 457 Programming Assignment 3
// Fall 2021
// Jacob Gayban
//
// use a (existing) database
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef USE_HPP
#define USE_HPP

extern string currentDB;

// USE db_name;

/**
 * @brief  Sets the current DB 
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processUse(istream *const line)
{
    string word;
    *line >> word;

    if (word.back() == ';')
        word.pop_back();

    if (fs::exists(word))
    {
        currentDB = word;
        cout << "Using database " << word << "." << endl;
    }
    else
    {
        cout << "!" << word << " is not an existing database." << endl;
        return false;
    }

    return true;
}

#endif
//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the BEGIN command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../classes/Transaction.h"
#include "../globals.hpp"

extern Transaction transaction;

bool startTransaction(istream *const line) {
    string word;
    *line >> word;

    if (word.back() == ';') word.pop_back();

    if (!transaction.is_active()) {
        if (capitalize(word) == "TRANSACTION") {
            cout << "Transaction starts." << endl;
            transaction.begin();
            return true;
        } else {
            // unexpected keyword error (throw exception?)
            return false;
        }
    } else {
        cout << "Transaction already started." << endl;
    }
    return false;
}
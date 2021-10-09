// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the DELETE command
// 

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef DELETE_HPP
#define DELETE_HPP

bool processDelete(istream* const line) {
    string word;

    // grab next word, expecting from
    *line >> word;

    switch(resolve(word)) {
        case FROM: {
            bool deleteAll = false;

            // extract table name
            *line >> word;

            if (word.back() == ';') word.pop_back();

            if (!currentDB.empty()) {
                if (fs::exists(currentDB + "/" + word)) {
                    Table table(word);
                    if (deleteAll) {
                        table.delete_all();
                    } else {
                        // grab the WHERE (not checking it rn)
                        *line >> word;
                        table.delete_where(read_condition(line, table));
                    }
                    table.printFile();
                } else {
                    cout << "!Failed to delete from " << word << " because it does not exist." << endl;
                }
            } else {
                cout << "!Cannot delete from table; no database in use." << endl;
            }
        }
    }
}

#endif 
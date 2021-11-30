//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the DROP command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;
namespace fs = std::filesystem;

#ifndef DROP_HPP
#define DROP_HPP

// DROP DATABASE db_name;
// DROP TABLE table_name;

/**
 * @brief  Deletes a database (or table)
 * @param  line Stream to get input from
 * 
 * @return True if operation succeeded
 */
bool processDrop(istream *const line)
{
    string word;

    // grab the qualifier
    *line >> word;

    switch (resolveWord(word))
    {

        // these could be brought out into their own functions

    case DATABASE:
    {
        *line >> word;
        if (word.back() == ';')
            word.pop_back();
        if (fs::remove_all(word))
        {
            cout << "Database " << word << " deleted." << endl;
        }
        else
        {
            cout << "!Failed to delete " << word << " because it does not exist." << endl;
        }
        break;
    }

    case TABLE:
    {
        // grab table name
        *line >> word;

        // title case table name
        word = title_case(word);

        if (word.back() == ';')
            word.pop_back();

        // similar to CREATE, we don't need to check
        // if the table exists or not
        // if (!currentDB.empty())
        // {
        //     if (fs::remove(currentDB + "/" + word))
        //     {
        //         cout << "Table " << word << " deleted." << endl;
        //     }
        //     else
        //     {
        //         cout << "!Failed to delete " << word << " because it does not exist." << endl;
        //     }
        // }
        // else
        // {
        //     cout << "!Cannot delete table; no database in use." << endl;
        // }

        if (!table_exists(word, "delete"))
            return false;

        // if the locked table is not ours, then ignore the command
        if (is_locked(word) && !transaction.owns(word))
        {
            cout << "Error: Table " << word << " is locked!" << endl;
            transaction.fail();
            // //line->ignore(numeric_limits<streamsize>::max(), ';');
            return false;
        }
        else
        {
            // actually this will delete the table file itself, not the table within the transaction object
            fs::remove(currentDB + "/" + word);
            cout << "Table " << word << " deleted." << endl;
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

    return false;
}

#endif
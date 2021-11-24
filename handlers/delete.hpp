//
// CS 457 Programming Assignment 3
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
#include "../classes/Table.hpp"
#include "../classes/Condition.h"

using namespace std;
namespace fs = std::filesystem;

#ifndef DELETE_HPP
#define DELETE_HPP

bool processDelete(istream *const line)
{
    string word;

    // grab next word, expecting from
    *line >> word;

    switch (resolveWord(word))
    {
    case FROM:
    {
        // represents if there is a condition present
        bool deleteAll = false;

        // extract table name
        *line >> word;

        // title case table name
        word = title_case(word);

        // if there is no WHERE clause, then delete all
        if (word.back() == ';')
        {
            word.pop_back();
            deleteAll = true;
        }

        // if the table doesn't exist, do nothing
        if (!table_exists(word, "delete from"))
            return false;

        // if the locked table is not ours, then ignore the command
        if (is_locked(word) && !transaction.owns(word))
        {
            cout << "Error: Table " << word << " is locked!" << endl;
            transaction.fail();
            line->ignore(numeric_limits<streamsize>::max(), ';');
            return false;
        }

        // table exists, remove elements
        Table table(word);

        // if a transaction exists, use the saved value
        if (transaction.is_active())
        {
            if (transaction.tables.count(word))
                table = transaction.tables[word];
            transaction.lock(word);
        }

        if (deleteAll)
        {
            table.delete_all();
        }
        else
        {
            // grab the WHERE (not checking it rn)
            *line >> word;
            table.delete_where(read_condition(line, table));
        }

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
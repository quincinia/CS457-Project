//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Describes a transaction in progress
//

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <fstream>
#include <filesystem>
#include "Transaction.h"

using namespace std;
namespace fs = std::filesystem;

// don't know if this is needed?
extern string currentDB;

bool Transaction::owns(string name)
{
    return tables.count(name);
}

void Transaction::commit()
{
    if (failed)
    {
        abort();
        return;
    }

    for (auto &t : tables)
    {
        // write to disk
        t.second.printFile();

        // unlock file
        unlock(t.first);
    }

    tables.clear();

    active = false;
    failed = false;
    cout << "Transaction committed." << endl;
}

void Transaction::abort()
{
    tables.clear();
    active = false;
    failed = false;
    cout << "Transaction abort." << endl;
}

bool Transaction::lock(string table)
{
    if (fs::exists(currentDB + "/" + table + "_lock"))
    {
        cout << "!Table " << table << " has already been locked." << endl;
        return false;
    }
    else
    {
        ofstream(currentDB + "/" + table + "_lock");
        cout << "Table " << table << " locked." << endl;
        return true;
    }
    return false;
}

void Transaction::unlock(string table)
{
    try
    {
        // if the table name does not exist, will throw an exception
        tables.at(table);

        // if no exception is thrown, then we own this table and can unlock it
        fs::remove(currentDB + "/" + table + "_lock");
        cout << "Removed file: " << table + "_lock" << endl;

        // unlocking will not save the table to disk, so be careful
    }
    catch (const exception)
    {
        cout << "!Unknown or unowned table " << table << endl;
    }
}

#endif
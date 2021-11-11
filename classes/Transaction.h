//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Describes a transaction in progress
//

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <map>
#include "Table.h"

using namespace std;

class Transaction
{
private:
    bool active = false;

    // if we encounter any locked file during the transaction, then the 
    // entire thing will fail even if other operations were successful
    bool failed = false;

    

public:
    // table names must be validated before calling this
    Table &get_table(string name);

    // for accessing and adding
    map<string, Table> tables;

    bool is_active() { return active; }
    void begin() { active = true; }
    void fail() { failed = true; }

    // saves all changes to disk
    // removes all locks this process owns
    void commit();

    // deletes all locks, but does not save table changes
    void abort();

    // creates a lock file for this table, and adds it to storage
    bool lock(string table);

    // removes the lock file for this table
    // keeps semantics consistent
    void unlock(string table);
};

#endif
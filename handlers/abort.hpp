//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the ABORT command
//

#ifndef ABORT_HPP
#define ABORT_HPP

#include "../classes/Transaction.h"

using namespace std;

extern Transaction transaction;

void processAbort(istream *const line)
{
    if (transaction.is_active())
        transaction.abort();
    else
        cout << "!Cannot abort; no transaction in progress" << endl;
}

#endif
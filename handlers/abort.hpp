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
    transaction.abort();
}

#endif
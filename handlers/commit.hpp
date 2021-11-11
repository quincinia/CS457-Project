//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the COMMIT command
//

#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <iostream>
#include "../classes/Transaction.h"

using namespace std;

extern Transaction transaction;

void processCommit(istream *const line)
{
    transaction.commit();
}

#endif
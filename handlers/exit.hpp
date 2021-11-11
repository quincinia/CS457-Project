//
// CS 457 Programming Assignment 3
// Fall 2021
// Jacob Gayban
//
// Performs any exit operations (if needed)
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;

#ifndef EXIT_HPP
#define EXIT_HPP

// .EXIT
// exit DOES NOT have a semicolon after it

/**
 * @brief  Performs exit operations (but won't end the program)
 * @return True if operation succeeded
 */
bool processExit()
{
    // transactions must be explicitly committed
    transaction.abort();
    cout << "All done." << endl;
    return true;
}

#endif
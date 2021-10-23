//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Basic database management system, supports a few commands
//

#include <iostream>
#include <sstream>
#include <filesystem>
#include "globals.hpp"
#include "parser.hpp"
#include "classes/Attribute.hpp"
#include "classes/Condition.hpp"
#include "classes/Table.hpp"

using namespace std;
namespace fs = std::filesystem;
// add "-lstdc++fs" at the end to compile with filesystem
// clang++-7 -pthread -std=c++17 -o main main.cpp -lstdc++fs

// To run a file, use:
// ./main <filename>

// Files can only be read ONCE, at the start of the program, after which
// the program switches to command-line-only mode

void table_test();

int main(int argc, char *argv[])
{
    string test;
    cout << boolalpha;

    // run commands from file (if possible)
    if (argc >= 2)
    {
        istream *file = new ifstream(argv[1]);
        parseStream(file);
        delete file;
    }

    cout << "!Notice: when using SET and WHERE, please format your input as follows:" << endl;
    cout << "<item> <operator> <value>" << endl;
    cout << "Note that this program IS whitespace-sensitive." << endl;
    cout << "Correct usage:   price = 14.99" << endl;
    cout << "Incorrect usage: price=14.99" << endl;

    // run commands from command line
    while (parseStream(&cin))
        ;
    // getline(cin, test);
}

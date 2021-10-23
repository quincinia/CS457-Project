//
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../globals.hpp"

using namespace std;

#ifndef COMMENT_HPP
#define COMMENT_HPP

/**
 * @brief  Ignore the rest of the line following a comment
 * @param  stream Stream to process
 * @pre    Comments should not be in the middle of a command
 * @return True if operation succeeded
 */
bool processComment(istream *const stream)
{
  stream->ignore(numeric_limits<streamsize>::max(), '\n');
  return true;
}

#endif
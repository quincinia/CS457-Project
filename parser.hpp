//
// CS 457 Programming Assignment 3
// Fall 2021
// Jacob Gayban
//
// Processes the input stream, calling specific handlers when appropriate
//

#include <iostream>
#include <sstream>
#include <filesystem>
#include "globals.hpp"
#include "handlers/alter.hpp"
#include "handlers/comment.hpp"
#include "handlers/create.hpp"
#include "handlers/delete.hpp"
#include "handlers/drop.hpp"
#include "handlers/exit.hpp"
#include "handlers/insert.hpp"
#include "handlers/select.hpp"
#include "handlers/update.hpp"
#include "handlers/use.hpp"

#ifndef PARSER
#define PARSER

/**
 * @brief  Parses a file until end, or cin, and executes statements within
 * @param  stream Stream to get input from 
 *   
 * @return True if processes were successful
 */
bool parseStream(istream *const stream)
{
    // istream pointer supports cin and fstream

    // process the entire line for commands
    while (stream->good())
    {

        // grab the first word in the line
        // (should be a keyword)
        string word;
        *stream >> word;

        //cout << word << endl;

        // process the rest of the line based
        // on the command

        // not all keywords are commands
        switch (resolveWord(word))
        {
        case ALTER:
        {
            processAlter(stream);
            break;
        }
        case COMMENT:
        {
            processComment(stream);
            break;
        }
        case CREATE:
        {
            processCreate(stream);
            break;
        }
        case DELETE:
        {
            processDelete(stream);
            break;
        }
        case DROP:
        {
            processDrop(stream);
            break;
        }
        case EXIT:
        {
            processExit();
            return false;
            break;
        }
        case INSERT:
        {
            processInsert(stream);
            break;
        }
        case SELECT:
        {
            processSelect(stream);
            break;
        }
        case UPDATE:
        {
            processUpdate(stream);
            break;
        }
        case USE:
        {
            processUse(stream);
            break;
        }
        case INVALID_KEYWORD:
        {
            cout << "!Unrecognized command \"" << word << "\"." << endl;
            return false;
            break;
        }
        default:
        {
            cout << "!Unexpected keyword \"" << word << "\"." << endl;
            return false;
            break;
        }
        }
    }
    return true;
}

#endif
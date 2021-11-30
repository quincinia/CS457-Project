//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Core definitions and important variables
//

#ifndef GLOBALS
#define GLOBALS

#include <filesystem>
#include "classes/Transaction.hpp"

using namespace std;
namespace fs = std::filesystem;

Transaction transaction;
string currentDB = "";

// assumes table and DB already exists
bool is_locked(string table)
{
    return fs::exists(currentDB + "/" + table + "_lock");
}

string capitalize(string s)
{
    for (auto &c : s)
        c = toupper(c);
    return s;
}

string title_case(string s)
{
    for (auto &c : s)
        c = tolower(c);
    if (!s.empty())
        s.front() = toupper(s.front());
    return s;
}

enum Keyword
{
    INVALID_KEYWORD,
    ABORT,
    ALTER,
    ADD,
    BEGIN,
    COMMENT,
    COMMIT,
    CREATE,
    DATABASE,
    DELETE,
    DROP,
    EXIT,
    FROM,
    INSERT,
    INTO,
    SELECT,
    SET,
    TABLE,
    UPDATE,
    USE,
    VALUES,
    WHERE
};

// not gonna mess with maps or constexprs
Keyword resolveWord(string word)
{
    // only supporting caps right now
    word = capitalize(word);
    if (word == "ABORT")
        return ABORT;
    if (word == "ALTER")
        return ALTER;
    if (word == "ADD")
        return ADD;
    if (word == "BEGIN")
        return BEGIN;
    if (word.substr(0, 2) == "--")
        return COMMENT;
    if (word == "COMMIT;") // commit is a one-liner, so we're expecting a semicolon
        return COMMIT;
    if (word == "CREATE")
        return CREATE;
    if (word == "DATABASE")
        return DATABASE;
    if (word == "DELETE")
        return DELETE;
    if (word == "DROP")
        return DROP;
    if (word == ".EXIT")
        return EXIT;
    if (word == "FROM")
        return FROM;
    if (word == "INSERT")
        return INSERT;
    if (word == "INTO")
        return INTO;
    if (word == "SELECT")
        return SELECT;
    if (word == "SET")
        return SET;
    if (word == "TABLE")
        return TABLE;
    if (word == "UPDATE")
        return UPDATE;
    if (word == "USE")
        return USE;
    if (word == "VALUES")
        return VALUES;
    if (word == "WHERE")
        return WHERE;

    return INVALID_KEYWORD;
}

enum Datatype
{
    INVALID_TYPE,
    CHAR,
    FLOAT,
    INT,
    VARCHAR
};

Datatype resolveType(string word)
{
    word = capitalize(word);
    if (word.substr(0, 4) == "CHAR")
        return CHAR;
    if (word == "FLOAT")
        return FLOAT;
    if (word == "INT")
        return INT;
    if (word.substr(0, 7) == "VARCHAR")
        return VARCHAR;

    return INVALID_TYPE;
}

bool table_exists(string table, string err_msg)
{
    // DB must be in use before accessing table
    if (!currentDB.empty())
    {
        // table must exist before accessing
        if (fs::exists(currentDB + "/" + title_case(table)))
        {
            return true;
        }
        else
        {
            // if table does not exist, print error
            cout << "!Failed to " << err_msg << " table " << table << " because it does not exist." << endl;
            return false;
        }
    }
    else
    {
        // if DB does not exist, print error
        cout << "!Cannot " << err_msg << " table; no database in use." << endl;
        return false;
    }
    return false;
}

#endif
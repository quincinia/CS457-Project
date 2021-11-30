//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Contains functions related to the SELECT command
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <vector>
#include <utility> // std::pair
#include "../globals.hpp"
#include "../classes/Table.hpp"
#include "../classes/Condition.h"

using namespace std;
namespace fs = std::filesystem;

#ifndef JOINS_HPP
#define JOINS_HPP

int read_FROM_clause(istream *const line, string &tbl1, string &alias1, string &tbl2, string &alias2)
{
    *line >> tbl1;

    // (1)
    // select from X; (no condition)
    if (tbl1.back() == ';')
    {
        return 1;
    }

    *line >> alias1;

    // (1)
    // will accept an alias for a single table
    // select from X x; (no condition)
    if (alias1.back() == ';')
    {
        tbl1.push_back(';');
        return 1;
    }

    // (1)
    // default/singular case: <tbl_name> WHERE
    if (capitalize(alias1) == "WHERE")
    {
        // cout << "Reading from " << tbl1 << endl;
        return 1;
    }

    // (3)
    // inner join with comma
    if (alias1.back() == ',')
    {
        alias1.pop_back();
        *line >> tbl2 >> alias2;
        // cout << "Inner joining: " << tbl1 << "/" << alias1 << " with " << tbl2 << "/" << alias2 << endl;
        return 3;
    }

    // because we now got rid of the singular and default join, we now move into the explicit joins
    *line >> tbl2;

    // (2)
    // default/singular case: <tbl_name> [<alias>] WHERE
    // just in case an alias is specified
    if (capitalize(tbl2) == "WHERE")
    {
        // cout << "Reading from " << tbl1 << endl;
        return 2;
    }

    *line >> alias2;

    // (4)
    // inner join
    if (capitalize(tbl2) == "INNER" && capitalize(alias2) == "JOIN")
    {
        // grab the real names
        *line >> tbl2 >> alias2;
        // cout << "Inner joining: " << tbl1 << "/" << alias1 << " with " << tbl2 << "/" << alias2 << endl;

        // read the ON, then a condition
        return 4;
    }

    // (5)
    // left outer join
    string parser;
    *line >> parser;
    if (capitalize(tbl2) == "LEFT" && capitalize(alias2) == "OUTER" && capitalize(parser) == "JOIN")
    {
        *line >> tbl2 >> alias2;
        // cout << "Left outer joining: " << tbl1 << "/" << alias1 << " with " << tbl2 << "/" << alias2 << endl;
        return 5;
    }
    cout << "!SELECT: syntax error" << endl;
    return -1;
}

// (1, 2)
// technically this isn't a join at all, just a singular table
// pretty must just copied from processSelect
void singular_join(istream *const line, string &tbl, vector<string> &cols)
{
    bool selectAll = false;
    // title case table name
    tbl = title_case(tbl);

    // determine if there is a condition
    if ((selectAll = (tbl.back() == ';')))
        tbl.pop_back();

    // if the table doesn't exist, do nothing
    if (!table_exists(tbl, "query"))
        return;

    // table exists, print tuples
    Table table(tbl);
    if (selectAll)
    {
        table.select(cols).print();
    }
    else
    {
        // extract the WHERE (not checking this rn)
        // *line >> tbl;
        table.select(cols, read_condition(line, table)).print();
    }
    return;
}

// inner and louter joins are pretty much the same thing; this function is just more general than the following two
void join(istream *const line, string &tbl1, string &alias1, string &tbl2, string &alias2, vector<string> &cols, bool louter)
{
    // confirm that both tables exist
    if (!table_exists(tbl1, "query"))
        return;
    if (!table_exists(tbl2, "query"))
        return;

    // instantiate both tables
    Table table1(title_case(tbl1), alias1), table2(title_case(tbl2), alias2);

    // grab the ON, not checking this
    string parser;
    *line >> parser;

    table1.join(table2, read_aliased_condition(line, table1, table2), louter).select(cols).print();
}

// (3, 4)
// inner join
void inner_join(istream *const line, string &tbl1, string &alias1, string &tbl2, string &alias2, vector<string> &cols)
{
    join(line, tbl1, alias1, tbl2, alias2, cols, false);
}

// (5)
// left outer join
// splitting these up to keep semantics consistent
void left_outer_join(istream *const line, string &tbl1, string &alias1, string &tbl2, string &alias2, vector<string> &cols)
{
    join(line, tbl1, alias1, tbl2, alias2, cols, true);
}
#endif
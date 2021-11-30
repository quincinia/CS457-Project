//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Defines the Condition class, which describes
// a boolean operation performed on an Attribute
//

#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <iostream>
#include <string>    // stoi, stof
#include <exception> // string conversions throw exceptions
#include <cstdio>    // sscanf
#include "Attribute.h"
#include "Condition.h"
#include "Table.h"
#include "../globals.hpp"

using namespace std;

Condition read_condition(istream *const line, Table &table)
{
    string col, op, value;

    // there must be a space on both sides of the operator
    // eg. a1 > 5, not a1>5
    *line >> col >> op >> value;

    if (value.back() == ';')
        value.pop_back();

    return Condition(table.query_attributes(col), op, value);
}

Condition read_aliased_condition(istream *const line, Table &table1, Table &table2)
{
    string attr1, op, attr2;

    // same rules as above
    *line >> attr1 >> op >> attr2;

    if (attr2.back() == ';')
        attr2.pop_back();

    char alias[20], col[20];

    // grab the item on the left hand side of the operator
    sscanf(attr1.c_str(), "%[^.].%s", alias, col);

    // find the specified attribute
    Attribute lhs;
    if (alias == table1.alias)
    {
        lhs = table1.query_attributes(col);
    }
    else if (alias == table2.alias)
    {
        lhs = table2.query_attributes(col);
    }

    // do the same for the right side
    sscanf(attr1.c_str(), "%[^.].%s", alias, col);
    Attribute rhs;
    if (alias == table1.alias)
    {
        rhs = table1.query_attributes(col);
    }
    else if (alias == table2.alias)
    {
        rhs = table2.query_attributes(col);
    }

    // validate both attributes
    if (lhs.getType() == INVALID_TYPE || rhs.getType() == INVALID_TYPE)
    {
        throw invalid_argument("!One or more invalid attributes detected, cannot proceed with operation");
    }

    // we will use the aliased name as the attribute name (see Table::join)
    lhs.setName(attr1);

    // if both attributes are valid, we can now return the condition
    // here we are not using Condition in the traditional sense; instead of storing an
    // Attribute and a value, we are storing two Attributes, just that one of them is
    // left in string form
    return Condition(lhs, op, attr2);
}

Condition::Condition(const Attribute attribute, string operation, string comp)
    : attribute(attribute), operation(operation), comp(comp)
{
}

bool Condition::resolve(string value)
{
    try
    {
        switch (attribute.getType())
        {
        case CHAR:
        {
            return applyCond<string>(value, comp);
            break;
        }

        case FLOAT:
        {
            return applyCond<float>(stof(value), stof(comp));
            break;
        }

        case INT:
        {
            return applyCond<int>(stoi(value), stoi(comp));
            break;
        }

        case VARCHAR:
        {
            return applyCond<string>(value, comp);
            break;
        }

        default:
        {
            // if type is invalid, throw an exception
            throw exception();
        }
        }
    }
    catch (exception &e)
    {
        cout << "!Conversion failure or invalid datatype; check data" << endl;
    }
    return false;
}

bool Condition::resolve(string val1, string val2)
{
    // for manual comparisons
    // good for when the comparison value always changes (like when doing joins)
    comp = val2;
    return resolve(val1);
}

template <typename T>
bool Condition::applyCond(T a, T b)
{
    // operation is guaranteed to have at least one character
    switch (operation.front())
    {
    case '<':
    {
        if (operation.back() == '=')
        {
            return a <= b;
        }
        else
        {
            return a < b;
        }
        break;
    }

    case '>':
    {
        if (operation.back() == '=')
        {
            return a >= b;
        }
        else
        {
            return a > b;
        }
        break;
    }

    case '=':
    {
        // cout << "comparing " << a << " and " << b << ": " << (a == b) << endl;
        return a == b;
        break;
    }

    case '!':
    {
        return a != b;
        break;
    }

    default:
    {
        cout << "!Unknown operator in use" << endl;
        return false;
    }
    }
}

#endif
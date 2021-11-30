//
// CS 457 Programming Assignment 4
// Fall 2021
// Jacob Gayban
//
// Declares the Attribute class, used to describe
// the name and type of a column in a table
//

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../globals.hpp"

using namespace std;

class Attribute
{
private:
    string name;
    Datatype type;
    unsigned size; // for CHAR and VARCHAR types
public:
    Attribute(string name, string type);
    Attribute() : name(""), type(INVALID_TYPE) {}
    string toString();

    void setName(string name) { this->name = name; }
    string getName() const { return name; }
    Datatype getType() const { return type; }
    unsigned getSize() const { return size; }
};

#endif
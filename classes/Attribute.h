//
// CS 457 Programming Assignment 2
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
  string toString();

  string getName()   const { return name; }
  Datatype getType() const { return type; }
  unsigned getSize() const { return size; }
};

#endif
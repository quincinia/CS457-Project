// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Defines the Attribute class, used to describe 
// the name and type of a column in a table
// 

#include <iostream>
#include <sstream>
#include <cstdio>
#include "../globals.hpp"

using namespace std;

#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP
 
class Attribute {
  private:
    string name;
    Datatype type;
    unsigned size; // for CHAR and VARCHAR types
  public: 
    Attribute(string name, string type);
    string toString();

    string   getName() const { return name; }
    Datatype getType() const { return type; }
    unsigned getSize() const { return size; }
};

/**
 * @brief  Constructor
 * @param  name Name of the attribute
 * @param  type datatype of the attribute
 * @pre    Name conflicts should occur before creating a new Attribute
 * @return 
 */
Attribute::Attribute(string name, string datatype)
  : name(name), type(resolveType(datatype)) {
    unsigned u = 0;
    switch (this->type) {
      case CHAR: {
        sscanf(datatype.c_str(), "%*4c(%u)", &u);
        break;
      }

      case VARCHAR: {
        sscanf(datatype.c_str(), "%*7c(%u)", &u);
        break;
      }

      default: {
        // getting rid of warnings
        // if type is invalid, throw an exception
      }
    }
    this->size = u;
}

/**
 * @brief  Returns the attribute as <name> <datatype>
 */
string Attribute::toString() {
  // could've just used a normal string but w.e
  stringstream output;
  output << (name + " ");
  switch (this->type) {
    case CHAR: {
      output << "char(" << this->size << ")";
      break;
    }

    case FLOAT: {
      output << "float";
      break;
    }

    case INT: {
      output << "int";
      break;
    }

    case VARCHAR: {
      output << "varchar(" << this->size << ")";
      break;
    }

    default: {
      // getting rid of warnings
    }
  }
  return output.str();
}

#endif 

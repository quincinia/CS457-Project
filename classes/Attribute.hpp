// 
// CS 457 Programming Assignment 1
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

    string   getName() { return name; }
    Datatype getType() { return type; }
    unsigned getSize() { return size; }
}

/**
 * @brief  Constructor
 * @param  name Name of the attribute
 * @param  type datatype of the attribute
 * @pre    Name conflicts should occur before creating a new Attribute
 * @return 
 */
Attribute::Attribute(string name, string type)
  : name(name), type(resolveType(type)) {
    unsigned u = 0;
    switch (this.type) {
      case CHAR: {
        sscanf(type.c_str(), "%*4c(%u)", &u);
        break;
      }

      case VARCHAR: {
        sscanf(type.c_str(), "%*7c(%u)", &u);
        break;
      }
    }
    this.size = u;
}

/**
 * @brief  Returns the attribute as <name> <datatype>
 */
string Attribute::toString() {
  stringstream output(name + " ");
  switch (this.type) {
    case CHAR: {
      output << "char(" << this.size << ")";
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
      output << "varchar(" << this.size << ")";
      break;
    }
  }
  return output.str();
}

#endif 

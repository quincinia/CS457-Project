// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Defines the Condition class, which describes
// a boolean operation performed on an Attribute
//
 
#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <iostream>
#include <string>     // stoi, stof
#include <exception>  // string conversions throw exceptions
#include "Attribute.h"
#include "Condition.h"
#include "Table.h"

using namespace std;

Condition read_condition(istream* const line, Table& table) {
  string col, op, value;
  *line >> col >> op >> value;
  if (value.back() == ';') value.pop_back();

  return Condition(table.query_attributes(col), op, value);
}

Condition::Condition(const Attribute attribute, string operation, string comp)
  : attribute(attribute), operation(operation), comp(comp) {

}

bool Condition::resolve(string value) {
  try {
    switch (attribute.getType()) {
      case CHAR: {
        return applyCond<string>(value, comp);
        break;
      }

      case FLOAT: {
        return applyCond<float>(stof(value), stof(comp));
        break;
      }

      case INT: {
        return applyCond<int>(stoi(value), stoi(comp));
        break;
      }

      case VARCHAR: {
        return applyCond<string>(value, comp);
        break;
      }

      default: {
        // if type is invalid, throw an exception
      }
    }
  } catch (exception& e) {
    cout << "!Conversion failure; check data" << endl;
  }
  return false;
}

template<typename T>
bool Condition::applyCond(T a, T b) {
  // operation is guaranteed to have at least one character
  switch (operation.front()) {
    case '<': {
      if (operation.back() == '=') {
        return a <= b;
      } else {
        return a < b;
      }
      break;
    }

    case '>': {
      if (operation.back() == '=') {
        return a >= b;
      } else {
        return a > b;
      }
      break;
    }

    case '=': {
      // cout << "comparing " << a << " and " << b << ": " << (a == b) << endl;
      return a == b;
      break;
    }

    case '!': {
      return a != b;
      break;
    }

    default: {
      cout << "!Unknown operator in use" << endl;
      return false;
    }
  }
}

#endif 
// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Defines the Condition class, which describes
// a boolean operation performed on an Attribute
// 

#include <iostream>
#include <string>     // stoi, stof
#include <exception>  // string conversions throw exceptions

using namespace std;
namespace fs = std::filesystem; 

#ifndef CONDITION_HPP
#define CONDITION_HPP



class Condition {
  private: 
    template<typename T>
    bool applyCond(T a, T b); 
    
  public:
    // Attribute that condition is checking
    const Attribute& attribute;

    // condition to evaluate
    string operation;

    // value to compare against
    string comp;

    Condition(const Attribute& attribute, string operation, string value);

    // compares the inputted value against the stored one
    bool resolve(string value);
};

Condition::Condition(const Attribute& attribute, string operation, string comp)
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
// 
// CS 457 Programming Assignment 2
// Fall 2021
// Jacob Gayban
//
// Declares the Condition class, which describes
// a boolean operation performed on an Attribute
//
 
#ifndef CONDITION_H
#define CONDITION_H

class Attribute;
class Table;

using namespace std;

class Condition {
  private: 
    template<typename T>
    bool applyCond(T a, T b); 
    
  public:
    // Attribute that condition is checking
    const Attribute attribute;

    // condition to evaluate
    string operation;

    // value to compare against
    string comp;

    Condition(const Attribute attribute, string operation, string value);

    // compares the inputted value against the stored one
    bool resolve(string value);
};

Condition read_condition(istream* const line, Table& table);


#endif
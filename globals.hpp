// 
// CS 457 Programming Assignment 1
// Fall 2021
// Jacob Gayban
//
// Core definitions and important variables
// 

#ifndef GLOBALS
#define GLOBALS

using namespace std;

extern string currentDB;

enum Keyword {
  INVALID_KEYWORD,
  ALTER,
  ADD,
  COMMENT,
  CREATE,
  DATABASE,
  DROP,
  EXIT,
  FROM,
  SELECT,
  TABLE,
  USE
};

string capitalize(string s) {
  for (auto& c : s)
    c = toupper(c);
  return s;
}

// not gonna mess with maps or constexprs
Keyword resolveWord(string word) {
  // only supporting caps right now
  word = capitalize(word);
  if (word == "ALTER")    return ALTER;
  if (word == "ADD")      return ADD;
  if (word.substr(0, 2) == "--")       return COMMENT;
  if (word == "CREATE")   return CREATE;
  if (word == "DATABASE") return DATABASE;
  if (word == "DROP")     return DROP;
  if (word == ".EXIT")    return EXIT;
  if (word == "FROM")     return FROM;
  if (word == "SELECT")   return SELECT;
  if (word == "TABLE")    return TABLE;
  if (word == "USE")      return USE;

  return INVALID_KEYWORD;
}

#endif
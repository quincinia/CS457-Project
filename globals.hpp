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
  DELETE,
  DROP,
  EXIT,
  FROM,
  INSERT,
  INTO,
  SELECT,
  SET,
  TABLE,
  UPDATE,
  USE,
  VALUES,
  WHERE
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
  if (word == "DELETE")   return DELETE;
  if (word == "DROP")     return DROP;
  if (word == ".EXIT")    return EXIT;
  if (word == "FROM")     return FROM;
  if (word == "INSERT")   return INSERT;
  if (word == "INTO")     return INTO;
  if (word == "SELECT")   return SELECT;
  if (word == "SET")      return SET;
  if (word == "TABLE")    return TABLE;
  if (word == "UPDATE")   return UPDATE;
  if (word == "USE")      return USE;
  if (word == "VALUES")   return VALUES;
  if (word == "WHERE")    return WHERE;


  return INVALID_KEYWORD;
}

#endif
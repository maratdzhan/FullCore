#pragma once

#include <typeinfo>


template < typename T >
void GetParam(T & _value, const std::string & s, const size_t number);
std::string GetStringParam(const std::string &inputString, size_t number);
std::string ReturnNumbers(std::string str);
void ToUpperFunct(std::string & str);
void PathPreparing(std::string & assumedPath);

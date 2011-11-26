#ifndef __STRING_SPLIT_H__
#define __STRING_SPLIT_H__

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);



#endif
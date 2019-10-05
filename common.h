#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <map>

bool exists(char const *name);

// trim from start (in place)
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

std::map<std::string, std::string> parseKV(const std::string &sz);

#endif // _COMMON_H_

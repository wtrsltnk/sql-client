#include "common.h"
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>
#include <sys/stat.h>

bool exists(char const *name)
{
    struct stat buffer;
    return (stat (name, &buffer) == 0);
}

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::map<std::string, std::string> parseKV(const std::string &sz)
{
    std::map<std::string, std::string> ret;
    std::string key;
    std::string value;
    char endDelim = ';';
    const char *s = sz.c_str();

    while(*s)
    {
        // skip all spaces
        while (*s && *s <= ' ')
        {
            s++;
        }

        // parse the key
        while(*s && *s != '=')
        {
            key.push_back(*s);
            ++s;
        }

        // if we quit due to the end of the string exit now
        if(!*s) break;

        // skip the "=" and all spaces
        while (*s && (*s == '=' || *s <= ' '))
        {
            s++;
        }

        if (!*s) break;

        if (*s == '\'')
        {
            endDelim = '\'';
        }

        // parse the value
        while(*s && *s != endDelim)
        {
            value.push_back(*s);
            ++s;
        }

        // skip the ";", the end delimiter and all spaces
        while (*s && (*s == endDelim || *s == ';' || *s <= ' '))
        {
            s++;
        }

        trim(key);
        trim(value);

        ret[key] = value;
        key.clear();
        value.clear();
        endDelim = ';';
    }
    return ret;
}

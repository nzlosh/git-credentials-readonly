#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <regex>

#define DEFAULT_FILENAME ".git-credentials-ro"

class Url
{
public:
    std::string scheme;
    std::string username;
    std::string password;
    std::string hostname;
    std::string port;
    std::string path;
};

class Param
{
public:
    std::string option;
    std::string filename;
};

typedef std::unordered_map<std::string, Url> git_conf;

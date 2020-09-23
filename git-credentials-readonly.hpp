#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <unordered_map>

#define DEFAULT_FILENAME ".git-credentials-ro"
#define VERSION "0.1"

class Url
{
public:
    Url() : protocol(""), username(""), password(""), host(""), path("") {}
    Url(std::string protocol, std::string username, std::string password, std::string host, std::string path) :
        protocol(protocol), username(username), password(password), host(host), path(path) {}
    std::string protocol;
    std::string username;
    std::string password;
    std::string host;
    std::string path;
    bool operator==(const Url& url) const;
};

class Param
{
public:
    std::string option;
    std::string filename;
    bool use_http_path;
};

typedef std::vector<Url> git_conf;

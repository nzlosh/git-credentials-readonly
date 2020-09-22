#include "git-credentials-readonly.hpp"

bool parse_credentials(std::string const & filename, git_conf& user_credentials)
{
    /*
    Regular expression to extract URL.  7 fields will be captured in a well formed URL.
    0 - Full URL
    1 - scheme
    2 - username (optional but required if password is supplied)
    3 - password (optional but required if username is supplied)
    4 - hostname
    5 - port (optional)
    6 - path (optional)
    */
    std::regex url_pattern("^(http|https)://(?:([^:]+):([^@]+)@)?([^:/]+)(?::([^/]+))?/(.*)?$");
    std::smatch match;

    std::ifstream in{filename};
    if (!in)
    {
        std::cerr << "Unable to open " << filename << std::endl;
        exit(1);
    }

    for (std::string line; std::getline(in, line);)
    {
        if(std::regex_search(line, match, url_pattern))
        {
            user_credentials[line] = Url{match[1], match[2], match[3], match[4], match[5], match[6]};
        }
    }
    return true;
}

Param* parse_arguments(int argc, char* argv[])
{
    auto* param = new Param{};

    std::string home{""};
    if (getenv("HOME") != 0)
    {
        home += getenv("HOME");
        home += "/";
    }

    // set default filename.
    param->filename = home + DEFAULT_FILENAME;

    std::string arg;
    for (size_t i = 1; i < argc; i++)
    {
        arg = argv[i];
        if (arg == "-f" || arg == "--file")
        {
            if (i + 1 < argc)
            {
                param->filename = argv[i + 1];
            }
        }
        if (arg == "get" || arg == "store" || arg == "erase")
        {
            param->option = arg;
        }
    }
    return param;
}

int main(int argc, char* argv[])
{
    git_conf git_credentials;
    git_conf user_credentials;
    size_t sep;

    Param* param = nullptr;
    param = parse_arguments(argc, argv);

    std::cout << param->filename << " " << param->option << std::endl;

    if (parse_credentials(param->filename, user_credentials))
    {
        for (auto &[k, v] : user_credentials)
        {
            std::cout
                    << "scheme=" << v.scheme << std::endl
                    << "hostname=" << v.hostname << std::endl
                    << "username=" << v.username << std::endl
                    << "password=" << v.password << std::endl
                    << "port=" << v.port << std::endl
                    << "path=" << v.path << std::endl;
        }
    }

    // parse stdin for git passed options.
    for (std::string line; std::getline(std::cin, line);)
    {
        sep = line.find("=");
        if (sep != line.npos)
        {
            std::cout << line.substr(0, sep) << " = " << line.substr(sep+1, line.npos) << std::endl;
            sep = line.npos;
        }
    }
/*
    for (const auto& [key, value] : git_credentials)
    {
        std::cout << key << " = " << value << "\n";
    }
*/
    if (param != nullptr) delete param;

    return 0;
}

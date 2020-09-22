#include "git-credentials-readonly.hpp"

void parse_user_credentials(std::string const & filename, git_conf& user_credentials)
{
    /*
    Regular expression to extract URL.  6 fields will be captured in a well formed URL.
    0 - Full URL
    1 - protocol
    2 - username (optional but required if password is supplied)
    3 - password (optional but required if username is supplied)
    4 - host (hostname and optionally port)
    5 - path (optional)
    */
    std::regex url_pattern("^(http|https)://(?:([^:]+):([^@]+)@)?([^:/]+(?::[^/]+)?)/(.*)?$");
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
            user_credentials.push_back(Url{match[1], match[2], match[3], match[4], match[5]});
        }
    }
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
    for (int i = 1; i < argc; i++)
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
        if (arg == "-v" || arg == "--version")
        {
            std::cout << argv[0] << " v" << VERSION << std::endl;
            exit(0);
        }
    }
    return param;
}

void parse_git_credentials(Url& git_credentials)
{
    // parse stdin for git passed options.
    std::string k, v;
    size_t sep;

    for (std::string line; std::getline(std::cin, line);)
    {
        if (line == "") break;
        sep = line.find("=");
        if (sep != line.npos)
        {
            k = line.substr(0, sep);
            v = line.substr(sep+1, line.npos);

            if (k == "protocol") git_credentials.protocol = v;
            if (k == "host") git_credentials.host = v;
            if (k == "path") git_credentials.path = v;
            if (k == "username") git_credentials.username = v;
        }
        sep = line.npos;
        k = v = "";
    }
}

void fill_in_git_credentials(Url& git_credentials, git_conf const & user_credentials)
{
    for (auto & user_credential : user_credentials)
    {
        if (git_credentials == user_credential)
        {
            git_credentials.username = user_credential.username;
            git_credentials.password = user_credential.password;
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    Url git_credentials{};
    git_conf user_credentials;

    Param* param = nullptr;
    param = parse_arguments(argc, argv);

    parse_user_credentials(param->filename, user_credentials);
    parse_git_credentials(git_credentials);

    fill_in_git_credentials(git_credentials, user_credentials);

    std::cout
            << "protocol=" << git_credentials.protocol << std::endl
            << "host=" << git_credentials.host << std::endl
            << "username=" << git_credentials.username << std::endl
            << "password=" << git_credentials.password << std::endl
            << "path=" << git_credentials.path << std::endl;

    if (param != nullptr) delete param;

    return 0;
}

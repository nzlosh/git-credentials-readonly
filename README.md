# git-credentials-readonly

A git helper to search for authentication credentials in a text file.  All requests by git to store
or erase entries are ignored.  This allows the user to control the file via other means such as a
configuration management system.

The file is composed of one url per line.  The url must be well formed.

```
<protocol>://<username>:<password>@<hostname>:<port>/<path>
```

Port, username, password and path are optional but omitting username and password may not make 
senses with this helper.


## Build

A C++ compiler that supports the C++17 standard is required (GCC v8 or greater should be sufficient).
Cmake greater than or equal to version 3.10.

The following commands will clone and build the project.  An executable should be created
in the project's `build` directory.

```
git clone https://github.com/nzlosh/git-credentials-readonly
mkdir git-credentials-readonly/build
cd git-credentials-readonly/build
cmake ..
make
```

## Installation

Copy `git-credentials-readonly` to directory location such as `/usr/local/bin/` or a directory appropriate to your system.

Configure git to use a credential helper

`git config --global credential.helper /usr/local/bin/git-credentials-readonly`

Enable credentials for a git source
`git config --global credential.https://github.com`


## Usage
```
    git-credentials-readonly [-f|--file <filename>] <git option>

Flags:
    f or file: A path to the file that is to be read for URLs.  Default location is $HOME/.git-credentials-ro

Arguments:
    git option: Git supplied option 'get', 'store' or 'erase'.  Store and erase are silently ignored by this helper.
```

# git-credentials-readonly

A git helper to search for authentication credentials in a text file.  All requests by git to store
or erase entries are ignored.  This allows the user to control the file via other means such as a
configuration management system.

The file is composed of one url per line.  The url must be well formed with a scheme, username,
password, hostname, port and path. Port, username, password and path are optional but may not make
senses.

```
<scheme>://<username>:<password>@<hostname>:<port>/<path>
```

## Usage
```
    git-credentials-readonly [-f|--file <filename>] <git option>

Flags:
    f or file: A path to the file that is to be read for URLs.

Arguments:
    git option: Git supplied option 'get', 'store' or 'erase'.  Store and erased are silently ignored by this helper.
```


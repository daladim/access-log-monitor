# access-log-supervisor
A tool that checks for unexpected logins (IP, username or time based) from system logs

# How to use
Populate a configuration file with your rules: add the IPs, usernames and timespans that look normal or suspicious. See `doc/config-example.yml` for an example. \
Note that timespans are ignored for the moment.

Run `./access-log-supervisor --config /your/config/file --type log-type /var/log/your-log`\
`type` is the `shortName` of the parser to use (e.g. `apache`, `auth`...)

See `./access-log-supervisor --help` for the available commands, options, and their default values.

## Input formats
Several log input formats are available. \
See the `src/parsers/` folder for a list of available parsers.

## Output
Various output formats are available (plain text, HTML...). \
See the `src/serializers/` folder for a list of available serializers. \
The output can be mailed in case the config files requires so.

# How to build
Building this project requires these dependancies:
* libcidr-dev
* libsqlite3-dev
* cmake (to build a static dependancy)

Do not forget to also fetch the submodules (`git submodule init; git submodule update`) so that you can build the required static dependancies.

Run `make` to build to app. \
Run `make test` to build the test suite, which can be run with `build/tests`.

Running this project requires these dependancies:
* libcidr
* libsqlite3
* (optionally) a working `sendmail` setup in case you want the program to be able to send you summary e-mails.

# Developer documentation
A Doxygen documentation is available. Run `make doc` to generate it. Open it via `doc/developer-documentation.html`

# Notes
This tool can look over-tested (if ever a concept of "too much testing" existed), since this began as an "exercise project" I made after following a course on software testing.

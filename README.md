# access-log-supervisor
A tool that checks for unexpected logins (IP, username or time based) from system logs

# How to use
Populate a configuration file with your rules: add the IPs, usernames and timespans that look normal or suspicious. See `doc/config-example.yml` for an example.

Run `./access-log-supervisor --config /your/config/file --input /var/log/your-log --type log-type`

See `./access-log-supervisor --help` for the available commands, options, and their default values.

## Input formats
Several input formats are available. See the list with `./access-log-supervisor --input-formats`.

## Output
By default, the standard output is used.
Other output formats are available (such as e-mail). See the list with `./access-log-supervisor --output-formats`.

# How to build
This project requires these dependancies:
* libcidr-dev
* libsqlite3-dev

Do not forget to also fetch the submodules (`git submodule init; git submodule update`) so that you can build the required static dependancies.

Run `make` to build to app. \
Run `make test` to build the test suite, which can be run with `build/tests`.

# Developer documentation
A Doxygen documentation is available. Run `make doc` to generate it. Open it via `doc/developer-documentation.html`

# Notes
This tool can look over-tested (if ever a concept of "too much testing" existed), since this began as an "exercise project" I made after following a course on software testing.

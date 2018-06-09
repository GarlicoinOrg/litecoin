#!/bin/sh
set -ex

# first arg is `-f` or `--some-option`
# or first arg is `something.conf`
if [ "${1#-}" != "$1" ] || [ "${1%.conf}" != "$1" ]; then
	set -- grlc_oneshot "$@"
fi

exec "$@"


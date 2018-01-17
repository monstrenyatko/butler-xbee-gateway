#!/bin/bash

set -e
set -x


if [ "$(id -u)" = '0' -a -n "$APP_USERNAME" -a "$APP_USERNAME" != 'root' ]; then
	exec gosu "$APP_USERNAME" "$BASH_SOURCE" "$@"
fi

exec "$@"

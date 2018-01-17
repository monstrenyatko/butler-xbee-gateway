#!/bin/bash

# Usage: file_env_opt VAR [DEFAULT]
file_env_opt() {
	local var="$1"
	local fileVar="${var}_FILE"
	local def="${2:-}"
	if [ -z "${!var:-}" ] && [ -z "${!fileVar:-}" ] && [ -z "${def:-}" ]; then
		return 0
	fi
	if [ "${!var:-}" ] && [ "${!fileVar:-}" ]; then
		echo >&2 "error: both $var and $fileVar are set (but are exclusive)"
		exit 1
	fi
	local val="$def"
	if [ "${!var:-}" ]; then
		val="${!var}"
	elif [ "${!fileVar:-}" ]; then
		val="$(< "${!fileVar}")"
	fi
	export "$var"="$val"
	unset "$fileVar"
}

file_env_opt 'BUTLER_XBEE_GW_JWT_KEY'

set -x
set -e

if [ -n "$BUTLER_XBEE_GW_GID" -a -n "$APP_USERNAME" -a "$APP_USERNAME" != 'root' ]; then
	groupmod --gid $BUTLER_XBEE_GW_GID $APP_USERNAME
	usermod --gid $BUTLER_XBEE_GW_GID $APP_USERNAME
fi

if [ -n "$BUTLER_XBEE_GW_UID" -a -n "$APP_USERNAME" -a "$APP_USERNAME" != 'root' ]; then
	usermod --uid $BUTLER_XBEE_GW_UID $APP_USERNAME
fi

if [ "$1" = 'butler-xbee-gateway-app' ]; then
	shift;
	exec /app-entrypoint.sh butler-xbee-gateway "$@"
fi

exec "$@"

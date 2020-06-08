#!/bin/bash
set -e

[ -z "$CONFIG_FILE" ] && CONFIG_FILE="-f /srv/osirose/server.json" || CONFIG_FILE="-f $CONFIG_FILE"
[ -z "$URL" ] && AUTOCONFIG_URL="" || AUTOCONFIG_URL="--url $URL"
[ -z "$AUTOCONFIG_URL" ] && AUTOCONFIG_URL="" || AUTOCONFIG_URL="--url $AUTOCONFIG_URL"
[ -z "$CORE_PATH" ] && CORE_PATH="" || CORE_PATH="--core_path $CORE_PATH"
[ -z "$DB_HOST" ] && DB_HOST="" || DB_HOST="--db_host $DB_HOST"
[ -z "$DB_PORT" ] && DB_PORT="" || DB_PORT="--db_port $DB_PORT"
[ -z "$MYSQL_DATABASE" ] && DB_NAME="" || DB_NAME="--db_name $MYSQL_DATABASE"
[ -z "$MYSQL_USER" ] && DB_USER_NAME="" || DB_USER_NAME="--db_user $MYSQL_USER"
[ -z "$MYSQL_PASSWORD" ] && DB_USER_PASSWORD="" || DB_USER_PASSWORD="--db_pass $MYSQL_PASSWORD"
[ -z "$SERVER_ISC_IP" ] && SERVER_ISC_IP="" || SERVER_ISC_IP="--isc_ip $SERVER_ISC_IP"
[ -z "$SERVER_ISC_PORT" ] && SERVER_ISC_PORT="" || SERVER_ISC_PORT="--isc_port $SERVER_ISC_PORT"
[ -z "$EXTERNAL_IP" ] && EXTERNAL_IP="" || EXTERNAL_IP="--external_ip $EXTERNAL_IP"
[ -z "$LISTEN_IP" ] && LISTEN_IP="" || LISTEN_IP="--client_ip $LISTEN_IP"

[ -z "$SERVER" ] && { echo "SERVER Env variable is not set exiting."; exit 1; }
[ -e "$SERVER" ] || { echo "$SERVER does not exist."; exit 2; }
exec ./"$SERVER" $CONFIG_FILE $CORE_PATH $LISTEN_IP $EXTERNAL_IP $AUTOCONFIG_URL $SERVER_ISC_IP $SERVER_ISC_PORT $DB_HOST $DB_PORT $DB_NAME $DB_USER_NAME $DB_USER_PASSWORD
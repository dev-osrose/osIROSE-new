#!/bin/bash

if [ -n $CONFIG_FILE ]; then
  CONFIG_FILE="-f $CONFIG_FILE"
else
  CONFIG_FILE="-f /config/login-server.json"
fi

if [ -n $URL ]; then
  AUTOCONFIG_URL="--url $URL"
else
  AUTOCONFIG_URL=""
fi

if [ -n $DB_HOST ]; then
  DB_HOST="--db_host $DB_HOST"
else
  DB_HOST=""
fi
if [ -n $DB_PORT ]; then
  DB_PORT="--db_port $DB_PORT"
else
  DB_PORT=""
fi
if [ -n $DB_NAME ]; then
  DB_NAME="--db_name $DB_NAME"
else
  DB_NAME=""
fi
if [ -n $DB_USER_NAME ]; then
  DB_USER_NAME="--db_user $DB_USER_NAME"
else
  DB_USER_NAME=""
fi
if [ -n $DB_USER_PASSWORD ]; then
  DB_USER_PASSWORD="--db_pass $DB_USER_PASSWORD"
else
  DB_USER_PASSWORD=""
fi

# Check to see if we have a server set
if [ -n $SERVER ]; then
  if [ -e $SERVER ]; then
      ./$SERVER $CONFIG_FILE $AUTOCONFIG_URL $DB_HOST $DB_PORT $DB_NAME $DB_USER_NAME $DB_USER_PASSWORD
  else
    echo "$SERVER does not exist."
    exit 1
  fi
else
  echo "SERVER Env variable is not set exiting."
  exit 2
fi

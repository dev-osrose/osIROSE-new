#!/bin/bash

if [ -n $CONFIG_FILE ]; then
  CONFIG_FILE="-f $CONFIG_FILE"
else
  CONFIG_FILE="-f /config/login-server.json"
fi

if [ -n $AUTOCONFIG_URL ]; then
  AUTOCONFIG_URL="--url $URL"
else
  AUTOCONFIG_URL=""
fi

# Check to see if we have a server set
if [ -n $SERVER ]; then
  if [ -e $SERVER ]; then
      ./$SERVER $CONFIG_FILE $AUTOCONFIG_URL
  else
    echo "$SERVER does not exist."
    exit 1
  fi
else
  echo "SERVER Env variable is not set exiting."
  exit 2
fi

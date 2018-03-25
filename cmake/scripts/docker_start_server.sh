#!/bin/bash

# Check to see if we have a server set
if [ -n $SERVER ]; then
  if [ -e $SERVER ]; then
    if [ -n $CONFIG_FILE ]; then
      ./$SERVER -f $CONFIG_FILE
    else
      ./$SERVER -f /config/login-server.json
    fi
  else
    echo "$SERVER does not exist."
    exit 1
  fi
else
  echo "SERVER Env variable is not set exiting."
  exit 2
fi

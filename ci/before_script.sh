#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
  export TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  mysql --user="root" -e 'create database osirose;'
  mysql --user="root" --database="osirose" --password="" < "$PROOT/Database/osirose.sql"
  mkdir build && cd build
  mkdir logs
else
  echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
  exit 1
fi    

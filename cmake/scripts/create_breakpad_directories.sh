#!/bin/bash

SYMBOL_PATH=$1/`cat $2 | awk '{ print $4 }'`
mkdir -p $SYMBOL_PATH
mv $3 $SYMBOL_PATH

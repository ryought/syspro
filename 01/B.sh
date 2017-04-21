#!/bin/sh
cd coreutils-8.9
find . -name "*.c" -exec wc -l {} \; | sort -nr | awk '{print $2}'

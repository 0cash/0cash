#!/bin/bash

set -e

docker build -f ./qa/Dockerfile.test -t 0cash/0cash-test .
docker run -it --rm 0cash/0cash-test ./qa/0cash/full_test_suite.py
docker run -it --rm 0cash/0cash-test ./qa/pull-tester/rpc-tests.sh

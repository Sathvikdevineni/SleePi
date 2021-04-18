#!/bin/bash
DIR="$( cd "$( dirname "$0" )" && pwd )"
cd ${DIR}/build
cmake ..
cmake --build .
cd ../bin
./SleePi
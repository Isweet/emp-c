#!/bin/bash

g++ -pthread -Wall -march=native -maes -mrdseed -std=c++11 -O3 -c empc.cpp -o empc.o
g++ -pthread -Wall -march=native -maes -mrdseed -std=c++11 -O3 -L/usr/local/lib -L/usr/local/opt/openssl/lib -lemp-tool -lgmp -lcrypto -shared empc.o -o libempc.dylib
rm empc.o

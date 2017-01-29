#!/bin/bash
./bin/jacobiser 1000 10 0.01 1
echo ''
echo 'thread number 1'
./bin/jacobipar 1000 10 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobipar 1000 10 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobipar 1000 10 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobipar 1000 10 0.01 4

echo ''
echo 'new test'
./bin/jacobiser 5000 10 0.01 1
echo ''
echo 'thread number 1'
./bin/jacobipar 5000 10 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobipar 5000 10 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobipar 5000 10 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobipar 5000 10 0.01 4

#!/bin/bash
./jacobiser 1000 10 0.01 1
echo ''
echo 'thread number 1'
./jacobipar 1000 10 0.01 1
echo ''
echo 'thread number 2'
./jacobipar 1000 10 0.01 2
echo ''
echo 'thread number 3'
./jacobipar 1000 10 0.01 3
echo ''
echo 'thread number 4'
./jacobipar 1000 10 0.01 4

echo ''
echo 'new test'
./jacobiser 5000 10 0.01 1
echo ''
echo 'thread number 1'
./jacobipar 5000 10 0.01 1
echo ''
echo 'thread number 2'
./jacobipar 5000 10 0.01 2
echo ''
echo 'thread number 3'
./jacobipar 5000 10 0.01 3
echo ''
echo 'thread number 4'
./jacobipar 5000 10 0.01 4

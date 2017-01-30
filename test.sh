#!/bin/bash
./bin/jacobiser 1000 10 0.01
echo ''
echo 'thread number 1'
./bin/jacobiPar 1000 10 0.01 1
./bin/jacobifor 1000 10 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiPar 1000 10 0.01 2
./bin/jacobifor 1000 10 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiPar 1000 10 0.01 3
./bin/jacobifor 1000 10 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiPar 1000 10 0.01 4
./bin/jacobifor 1000 10 0.01 4

echo ''
echo 'new test'
./bin/jacobiser 1000 1000 0.01
echo ''
echo 'thread number 1'
./bin/jacobiPar 1000 1000 0.01 1
./bin/jacobiFor 1000 1000 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiPar 1000 1000 0.01 2
./bin/jacobiFor 1000 1000 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiPar 1000 1000 0.01 3
./bin/jacobiFor 1000 1000 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiPar 1000 1000 0.01 4
./bin/jacobiFor 1000 1000 0.01 4

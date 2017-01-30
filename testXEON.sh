#!/bin/bash
./bin/jacobiserXEON 1000 10 0.01
echo ''
echo 'thread number 1'
./bin/jacobiParXEON 1000 10 0.01 1
./bin/jacobiforXEON 1000 10 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiParXEON 1000 10 0.01 2
./bin/jacobiforXEON 1000 10 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiParXEON 1000 10 0.01 3
./bin/jacobiforXEON 1000 10 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiParXEON 1000 10 0.01 4
./bin/jacobiforXEON 1000 10 0.01 4

echo ''
echo 'new test'
./bin/jacobiserXEON 1000 1000 0.01
echo ''
echo 'thread number 1'
./bin/jacobiParXEON 1000 1000 0.01 1
./bin/jacobiForXEON 1000 1000 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiParXEON 1000 1000 0.01 2
./bin/jacobiForXEON 1000 1000 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiParXEON 1000 1000 0.01 3
./bin/jacobiForXEON 1000 1000 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiParXEON 1000 1000 0.01 4
./bin/jacobiForXEON 1000 1000 0.01 4

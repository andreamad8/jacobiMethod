#!/bin/bash
echo ''
echo 'thread number 1'
./bin/jacobiParPHI 1000 10 0.01 1
./bin/jacobiforPHI 1000 10 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiParPHI 1000 10 0.01 2
./bin/jacobiforPHI 1000 10 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiParPHI 1000 10 0.01 3
./bin/jacobiforPHI 1000 10 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiParPHI 1000 10 0.01 4
./bin/jacobiforPHI 1000 10 0.01 4

echo ''
echo 'new test'
echo ''
echo 'thread number 1'
./bin/jacobiParPHI 1000 1000 0.01 1
./bin/jacobiForPHI 1000 1000 0.01 1
echo ''
echo 'thread number 2'
./bin/jacobiParPHI 1000 1000 0.01 2
./bin/jacobiForPHI 1000 1000 0.01 2
echo ''
echo 'thread number 3'
./bin/jacobiParPHI 1000 1000 0.01 3
./bin/jacobiForPHI 1000 1000 0.01 3
echo ''
echo 'thread number 4'
./bin/jacobiParPHI 1000 1000 0.01 4
./bin/jacobiForPHI 1000 1000 0.01 4

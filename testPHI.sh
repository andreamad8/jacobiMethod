#!/bash
echo ''
echo 'thread number 1'
./jacobiParPHI 1000 10 0.01 1
./jacobiForPHI 1000 10 0.01 1
echo ''
echo 'thread number 2'
./jacobiParPHI 1000 10 0.01 2
./jacobiForPHI 1000 10 0.01 2
echo ''
echo 'thread number 3'
./jacobiParPHI 1000 10 0.01 3
./jacobiForPHI 1000 10 0.01 3
echo ''
echo 'thread number 4'
./jacobiParPHI 1000 10 0.01 4
./jacobiForPHI 1000 10 0.01 4

echo ''
echo 'new test'
echo ''
echo 'thread number 1'
./jacobiParPHI 1000 1000 0.01 1
./jacobiForPHI 1000 1000 0.01 1
echo ''
echo 'thread number 2'
./jacobiParPHI 1000 1000 0.01 2
./jacobiForPHI 1000 1000 0.01 2
echo ''
echo 'thread number 3'
./jacobiParPHI 1000 1000 0.01 3
./jacobiForPHI 1000 1000 0.01 3
echo ''
echo 'thread number 4'
./jacobiParPHI 1000 1000 0.01 4
./jacobiForPHI 1000 1000 0.01 4

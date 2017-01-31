#!/bash
./bin/jacobiParPHI 100   500 0.0001 90 10 > ris/ParPHI_100_500_0001
./bin/jacobiParPHI 500   500 0.0001 90 10 > ris/ParPHI_500_500_0001
./bin/jacobiParPHI 1000  500 0.0001 90 10 > ris/ParPHI_1000_500_0001
./bin/jacobiParPHI 5000  500 0.0001 90 10 > ris/ParPHI_5000_500_0001
./bin/jacobiParPHI 10000 500 0.0001 90 10 > ris/ParPHI_10000_500_0001

./bin/jacobiForPHI 100   500 0.0001 90 10 > ris/ForPHI_100_500_0001
./bin/jacobiForPHI 500   500 0.0001 90 10 > ris/ForPHI_500_500_0001
./bin/jacobiForPHI 1000  500 0.0001 90 10 > ris/ForPHI_1000_500_0001
./bin/jacobiForPHI 5000  500 0.0001 90 10 > ris/ForPHI_5000_500_0001
./bin/jacobiForPHI 10000 500 0.0001 90 10 > ris/ForPHI_10000_500_0001

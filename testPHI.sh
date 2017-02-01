#!/bash
./bin/jacobiserPHI 100   50 0.000001 > ris/serPHI_0100_500_0001
./bin/jacobiserPHI 500   50 0.000001 > ris/serPHI_0500_500_0001
./bin/jacobiserPHI 1000  50 0.000001 > ris/serPHI_01000_500_0001
./bin/jacobiserPHI 5000  50 0.000001 > ris/serPHI_05000_500_0001
./bin/jacobiserPHI 7000  50 0.000001 > ris/serPHI_07000_500_0001
./bin/jacobiserPHI 10000 50 0.000001 > ris/serPHI_010000_500_0001
./bin/jacobiserPHI 15000 50 0.000001 > ris/serPHI_015000_500_0001

./bin/jacobiParPHI 100   50 0.000001 90 5 > ris/ParPHI_0100_500_0001
./bin/jacobiParPHI 500   50 0.000001 90 5 > ris/ParPHI_0500_500_0001
./bin/jacobiParPHI 1000  50 0.000001 90 5 > ris/ParPHI_01000_500_0001
./bin/jacobiParPHI 5000  50 0.000001 90 5 > ris/ParPHI_05000_500_0001
./bin/jacobiParPHI 7000  50 0.000001 90 5 > ris/ParPHI_07000_500_0001
./bin/jacobiParPHI 10000 50 0.000001 90 5 > ris/ParPHI_010000_500_0001
./bin/jacobiParPHI 15000 50 0.000001 90 5 > ris/ParPHI_015000_500_0001

./bin/jacobiForPHI 100   50 0.000001 90 5 > ris/ForPHI_0100_500_0001
./bin/jacobiForPHI 500   50 0.000001 90 5 > ris/ForPHI_0500_500_0001
./bin/jacobiForPHI 1000  50 0.000001 90 5 > ris/ForPHI_01000_500_0001
./bin/jacobiForPHI 5000  50 0.000001 90 5 > ris/ForPHI_05000_500_0001
./bin/jacobiForPHI 7000  50 0.000001 90 5 > ris/ForPHI_07000_500_0001
./bin/jacobiForPHI 10000 50 0.000001 90 5 > ris/ForPHI_010000_500_0001
./bin/jacobiForPHI 15000 50 0.000001 90 5 > ris/ForPHI_015000_500_0001

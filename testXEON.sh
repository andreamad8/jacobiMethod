#!/bin/bash
./bin/jacobiParXEON 100 500 0.0001 17 1   > ris/ParXEON_100_500_0001
./bin/jacobiserXEON 100 500 0.0001        > ris/serXEON_100_500_0001
./bin/jacobiParXEON 500 500 0.0001 17 1   > ris/ParXEON_500_500_0001
./bin/jacobiserXEON 500 500 0.0001        > ris/serXEON_500_500_0001
./bin/jacobiParXEON 1000 500 0.0001 17 1  > ris/ParXEON_1000_500_0001
./bin/jacobiserXEON 1000 500 0.0001       > ris/serXEON_1000_500_0001
./bin/jacobiParXEON 5000 500 0.0001 17 1  > ris/ParXEON_5000_500_0001
./bin/jacobiserXEON 5000 500 0.0001       > ris/serXEON_5000_500_0001
./bin/jacobiParXEON 10000 500 0.0001 17 1 > ris/ParXEON_10000_500_0001
./bin/jacobiserXEON 10000 500 0.0001      > ris/serXEON_10000_500_0001

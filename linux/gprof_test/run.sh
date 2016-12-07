# /bin/bash
g++ -g main.cpp -pg
./a.out
gprof -l > tmp.txt
rm *.o -rf

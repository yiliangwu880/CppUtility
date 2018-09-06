
#!/bin/sh

make clean &&
make &&
sh stop.sh &&
./test.bin
#!/bin/bash

# Delete old peak-mem-gcc
rm -f ./peak-mem-cache-gcc

# Run the 'make' command
make

# Clean cache
sync; echo 3 > /proc/sys/vm/drop_caches

# Execute the './peak-mem-gcc' command
./peak-mem-cache-gcc

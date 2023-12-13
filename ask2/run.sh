#!/bin/bash

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No color

# Define the programs to run with their respective commands
commands=("./bin/integral_mc_seq" "./bin/integral_mc_shm 12" "./bin/integral_mc_shm_sem 12")

# make the files
make
echo -e "${GREEN}----------------------------------------${NC}"

# Run the files sequentially
for cmd in "${commands[@]}"
do
    echo -e "${BLUE}Starting $cmd${NC}"
    # start_time=$SECONDS
    $cmd > "${cmd// /_}.out" # Redirect output to a file
    # elapsed=$((SECONDS - start_time))
    # echo -e "${RED}Time taken for $cmd: ${elapsed} seconds${NC}"
done

echo -e "${GREEN}----------------------------------------${NC}"

# Print the results
for cmd in "${commands[@]}"
do
    echo -e "${GREEN}Results of $cmd:${NC}"
    cat "${cmd// /_}.out" # Display the content of the output file
    echo
done

# remove the files
make clean

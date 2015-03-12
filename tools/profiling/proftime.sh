#!/bin/bash


NUM_PARTICLES="000500 001000 002000 004000 008000 016000 032000 064000 128000 256000 512000"
#NUM_PARTICLES="000500 001000"
NUM_FRAMES=5000
NUM_EMITTERS=1

> captures/time.txt

for numparticles in $NUM_PARTICLES; do
    echo $numparticles

    filename=$1-${numparticles}-${NUM_EMITTERS}-${NUM_FRAMES} 
    /usr/bin/time -v bin/profile $numparticles ${NUM_EMITTERS} ${NUM_FRAMES} >& captures/$filename.txt
    head captures/$filename.txt -n 1 >> captures/time.txt
    tail -n +2 captures/$filename.txt > captures/time-$filename.txt
done

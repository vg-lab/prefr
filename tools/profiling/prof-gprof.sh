#!/bin/bash


NUM_PARTICLES="000500 001000 002000 004000 008000 016000 032000 064000 128000 256000 512000"
#NUM_PARTICLES="000500 512000"
NUM_FRAMES=5000
NUM_EMITTERS=1

for numparticles in $NUM_PARTICLES; do
    echo $numparticles

    filename=$1-${numparticles}-${NUM_EMITTERS}-${NUM_FRAMES}
 
    bin/profile $numparticles ${NUM_EMITTERS} ${NUM_FRAMES} #>& /dev/null
    gprof bin/profile > captures/gprof-$filename.txt  
    cat captures/gprof-$filename.txt | python ~/tools/gprof2dot.py -s -w > captures/gprof2dot-$filename.txt
    cat captures/gprof2dot-$filename.txt | dot -Tpng -o captures/graph-$filename.png
    mv gmon.out captures/gmon-$filename.out
done

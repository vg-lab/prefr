#!/bin/bash


NUM_PARTICLES="000500 001000 002000 004000 008000 016000 032000 064000 128000 256000 512000"
#NUM_PARTICLES="000500"
NUM_FRAMES=5000
NUM_EMITTERS=1

for numparticles in $NUM_PARTICLES; do
    echo $numparticles

    filename=$1-${numparticles}-${NUM_EMITTERS}-${NUM_FRAMES}
 
    valgrind --tool=callgrind bin/profile $numparticles ${NUM_EMITTERS} ${NUM_FRAMES} #>& /dev/null
    cgfile=$(find . -name 'callgrind.out.*' -type f | cut -d '/' -f2)
    echo $cgfile
    python ~/tools/gprof2dot.py -f callgrind -s -w $cgfile > captures/valgrind/gprof2dot-$filename.txt
    cat captures/valgrind/gprof2dot-$filename.txt | dot -Tsvg -o captures/valgrind/vg-$filename.svg
    mv $cgfile captures/valgrind/callgrind-$filename.out
done

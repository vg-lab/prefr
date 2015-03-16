#!/bin/bash


NUM_PARTICLES="000500 001000 002000 004000 008000 016000 032000 064000 128000 256000 512000 1024000"
#NUM_PARTICLES="000500 001000"
NUM_FRAMES=500
NUM_EMITTERS=1
BRANCHES="master cachedParticleModifiers sortReduction"
TESTS="std cuda"
COMPOPTIONS="OFF ON"


for branch in $BRANCHES; do

	direc=captures/$branch
	echo $branch
	
	if [ ! -d "$direc" ]; then
		mkdir $direc
	fi

	git checkout $branch

	for i in $(seq 1 2); do

		#echo $TESTS | cut -d" " -f$i
		testname=$(echo $TESTS | cut -d" " -f$i)
		echo $testname

		#echo $COMPOPTIONS | cut -d" " -f$i
		opt=$(echo $COMPOPTIONS | cut -d" " -f$i)
		echo $opt		

		cmake . -DWITH_CUDA=$opt
		make -j 2
		fulldir=$direc/$testname
		echo $fulldir

		if [ ! -d "$fulldir" ]; then
			mkdir $fulldir
		fi

		> $fulldir/time.txt

		for numparticles in $NUM_PARTICLES; do
			echo $numparticles

	  		filename=$testname-$branch-${numparticles}-${NUM_EMITTERS}-${NUM_FRAMES} 
			/usr/bin/time -v bin/profile $numparticles ${NUM_EMITTERS} ${NUM_FRAMES} >& $fulldir/$filename.txt
			head $fulldir/$filename.txt -n 1 >> $fulldir/time.txt
		    	#tail -n +2 $fulldir/$filename.txt > $fulldir/time-$filename.txt
		done
	done
done

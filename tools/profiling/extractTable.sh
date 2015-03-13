

for i in `ls gprof2*txt | head -1 `; do 
    echo $i | cut -d"-" -f2
    echo $i | cut -d"-" -f4
    echo $i | cut -d"-" -f5 | cut -d. -f1
done

    echo "Particles number,SetupRender,UpdateCameraDistances,UpdateRender,UpdateUnified"

for i in `ls gprof2*txt`; do 

    grep -e UpdateUnified -e UpdateCameraDistances -e UpdateRender -e SetupRender $i  |\
        cut -d "\n" -f5 #| sed -e "s/\\\n/#/g" | cut -d"#" -f1,2 | sed -e "s/#/ /"  #|\
        #sed -e "s/%//" | sort  | cut -d" " -f2 |\
        #awk  -v file=$i -v parts=`echo $i | cut -d- -f3 ` 'BEGIN{ printf("%d,", parts ) } {printf("%s,", $0) } END{ print "" }'  

done

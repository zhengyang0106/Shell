#!/bin/bash

Max=0
MaxString=0

function Fliter() {
	FliterFile=$1
    file ${FliterFile} | grep -q -w "text"
	if [[ $? -eq 0 ]]; then
		echo 0
	else
		echo 1
	fi
}


function FindFile() {
	local File=$1
	for i in `cat ${File} | tr -c -s "A-Za-z" "\n"`; do
        #echo $i
		if [[ ${Max} -lt ${#i} ]]; then
			Max=${#i}
			MaxString=${i}
		fi
	done
}


function FindDir() {
	local DirName=$1
	for i in `ls -A ${DirName}`; do
		if [[ -d ${DirName}/${i} ]]; then
			FindDir ${DirName}/${i}
		elif [[ -f ${DirName}/${i} ]]; then
			Check=`Fliter ${DirName}/${i}`
			if [[ ${Check} -eq 0 ]]; then
				FindFile ${DirName}/${i}
           # else exit 0
			fi
		fi
	done
}


for i in $@; do 
#循环参数列表
if [[  -d $i ]]; then
	FindDir $i

    printf "MaxString=%s, Max=%d\n" ${MaxString} ${Max}
    MaxString=0
    Max=0

elif [[ -f $i ]]; then
	Check=`Fliter $i`
	if [[ ${Check} -eq 0 ]]; then
		FindFile $i

    printf "MaxString=%s, Max=%d\n" ${MaxString} ${Max}
    Max=0
    MaxString=0
    else exit 0

    fi
else exit 0
fi

#printf "MaxString=%s, Max=%d\n" ${MaxString} ${Max}
done

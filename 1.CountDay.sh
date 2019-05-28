#!/bin/bash

sum_day=0
year=$1
year_prime=365
year_he=366
sum=7
for (( i=0 ; i<$2; i++ )); do
    if [[ year%400 -eq 0 || year%4 -eq 0 && yaer%100 -ne 0 ]]; then
        sum_day=$[ ${sum_day}+365 ]
    
    else sum_day=$[ ${sum_day}+366 ]
    fi

    year=$[ ${year} + 1 ]

done
printf "%d\n" ${sum_day}

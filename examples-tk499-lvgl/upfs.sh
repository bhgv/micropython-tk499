#!/bin/bash

IFS=$'\n'

#echo "lpwd Script to upload examples using mpfshell" > upload.mpf
#echo >> upload.mpf

#for ln in $(bash mk_upload.sh)
#do
#  echo $ln >> upload.mpf
#done

PORT=ttyACM0

rm *.mpf

bash mk_upload.sh

i=0
while [ "$(ls up.${i}.mpf)" != "" ];
do
  sleep 1
  echo $i
  mpfshell -o ${PORT} -s up.${i}.mpf
  i=$((i+1))
done

rm *.mpf

#mpfshell -o ttyACM0 -s upload.mpf

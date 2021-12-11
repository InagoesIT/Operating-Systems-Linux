#! /bin/bash
#Suma primelor k nr divizibile cu d#

if [ $# -lt 2 ]
then
    read -p "Dati nr k: " k
    read -p "Dati nr d: " d
else
    k=$1
    d=$2
fi

declare -i sum
declare -i divizor
divizor=$d

for i in `seq 1 $k`
do
    sum=$sum+$divizor
    divizor=$divizor+$d
done

echo "Suma primelor $k numere divizibile cu $d este: $sum"     

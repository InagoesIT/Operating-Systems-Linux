#!/bin/bash
##Factorial recursiv##

if [ $# -eq 0 ]
then
    read -p "Dati n " n
else
    n=$1
fi

function factorial()
{
    if [ $1 -le 1 ]
    then
        echo $2
    else
        let n_1=$1-1
        let prod=$1*$2
        factorial $n_1 $prod
     fi
}

echo -n "Rez la factorial de $n: "
factorial $n 1

#! /bin/bash
#Fibonacci Recursiv#

if [ $# -eq 0 ]
then
    read -p "Dati n: " n
else
    n=$1
fi

function fibo()
{
    n=$1
    
    if [ $n -eq 0 ]
    then
        echo "0"
    elif [ $n -eq 1 ]
    then
        echo "1"
    else
        n_1=`expr $n - 1`
        n_2=`expr $n - 2`
        echo `expr $(fibo $n_1) + $(fibo $n_2)`
    fi
}

echo "Fibo de $n este: $(fibo $n)"

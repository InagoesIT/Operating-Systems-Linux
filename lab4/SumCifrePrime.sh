#! /bin/bash
## Suma cifrelor prime a unui nr recursiv ##

if [ $# -lt 1 ]
then
    read -p "Dati n: " n
else
    let n=$1
fi

if [ $n -le 0 ]
then
    exit 1
fi

sum=0

function isPrime()
{
    if [ $1 -le 1 ]
    then
        return 0
    else
        for (( i=2; i<$1; i++ ))
        do
            if [ `expr $1 % $i` -eq 0 ]
            then
                return 0
            fi
        done
        return 1
    fi
}

function SumCifre ()
{
    if [ $n -le 0 ]
    then
        echo -n "$2"
    else
        let n=$1
        let sum=$2
        let mod=$n%10
        let n=$n/10
        isPrime $mod
        if [ $? -eq 1 ]
        then
            let sum+=$mod
        fi
        SumCifre $n $sum        
    fi     
}

echo -n "Suma cifrelor lui $n este: "
SumCifre $n $sum
echo ""

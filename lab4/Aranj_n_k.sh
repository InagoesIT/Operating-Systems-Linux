#! /bin/bash
## Aranjamente de n luate cate k ##

if [ $# -lt 2 ]
then
    read -p "Dati n: " n
    read -p "Dati k: " k
else
    n=$1
    k=$2
fi

if [ $n -le $k ]
then
    echo "k este mai mare sau egal cu n!"
    exit 1
fi

prod=1

for (( nr=$n-$k+1; nr<=$n; nr++ ))
do
   let prod*=nr
done

echo "Aranjamente de $n luate cate $k = $prod"     

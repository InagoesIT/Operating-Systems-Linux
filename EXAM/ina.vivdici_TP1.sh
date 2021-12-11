#!/bin/bash

if (( $# == 0 ))
then
    read -p "Dati numele directorului: " $dir
elif [ ! -d $1 -o ! -r $1 ] 
then
    echo "Directorul introdus nu exista sau nu aveti drepturi de acces asupra lui!"
    exit 1
else
    dir=$1
fi

for line in $(cat /etc/group | cut -f3 -d:)
do
    mkdir $line
done

for stud in $(cat /etc/passwd | cut -f1,3 -d:)
do
    name=$(echo $stud | cut -f1 -d:)
    group=$(echo $stud | cut -f3 -d:)
    touch "./$group/$name.txt"
done

studList=$(cat /etc/passwd | cut -f1 -d:)

for user in studList
do 
    nrFis=0
    for source in $(find . $dir -user $user)
        do
            if [ ! -d source ]
            then
                nrFis+=1
            fi
    done
    echo $nrFis > "./$group/$name.txt"
done
       

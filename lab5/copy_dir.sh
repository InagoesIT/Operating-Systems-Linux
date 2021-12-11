#!/bin/bash

if (( $# == 0 ))
then
    read -p "Dati numele directorului" $dir
elif [ ! -d $1 -o ! -r $1 ] 
then
    echo "Directorul introdus nu exista sau nu aveti drepturi de acces asupra lui!"
    exit 2
else
    dir=$1
fi

dir=$1

for source in $(find $dir -type f -name "*.sh")
do
    newName=$(md5sum $fis | cut -f1 -d" ")
    cp $source $dir/samples/$newName
done    

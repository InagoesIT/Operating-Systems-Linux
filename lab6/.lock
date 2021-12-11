#! /bin/bash
#Concatenarea la toate .sh in final_script.sh din dir#

if [ $# -eq 0 ]
then
    read -p "Dati numele directorului: " dir
else
    dir=$1
fi

function parcurgere_dir()
{
    for fis in $(ls -A $1)
    do
        cale=$1/$fis
        
        if [ -d $cale ]
        then
            parcurgere_dir $cale
        else
            let lastPoz=${#fis}-1
            let firstPoz=len-3
            if [ ${fis:$firstPoz:$lastPoz} = ".sh" ]
            then
                cat $cale >> $dir/final_script.sh
            fi
        fi
    done
}

parcurgere_dir $dir

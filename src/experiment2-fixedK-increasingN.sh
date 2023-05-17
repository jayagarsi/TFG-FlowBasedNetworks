#!/bin/bash

runMultipleExperiments() {
    k=$1
    m=$2
    p=$3
    maxN=$4
    s=$5
    graphType=$6
    filePath=$7
    parametersFile="./dynamicsParameters.txt"
    agentsToAdd=0
    agentsToRemove=0
    modification="none"
    orders="rr ra"

    for order in $orders
    do
        # Min-Flow model
        model="min"
        csvFile="$model""Flow-$order-k$k-m$m-s$s.csv"
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((n=3; n<=maxN; n++)) do
            echo "--------- EXPERIMENT WITH N $N ---------"
            dynamicsImageFile=$model"Flow-"$order"-n"$n".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > $parametersFile
            ./dynamics
            case $order in
                ra)
                    mv $dynamicsImageFile $filePath"/Random"
                    ;;
                rr)
                    mv $dynamicsImageFile $filePath"/RoundRobin"
                    ;;
                *)
                    echo "Something went wrong while moving the result image"
                    ;;
            esac
        done
        mv -vn $csvFile $filePath
        rm -f $parametersFile

        # Avg-Flow model
        model="avg"
        csvFile="$model""Flow-$order-k$k-m$m-s$s"
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((n=3; n<=maxN; n++)) do
            echo "--------- EXPERIMENT WITH N $N ---------"
            dynamicsImageFile=$model"Flow-"$order"-n"$n".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > $parametersFile
            ./dynamics
            case $order in
                ra)
                    mv $dynamicsImageFile $filePath"/Random"
                    ;;
                rr)
                    mv $dynamicsImageFile $filePath"/RoundRobin"
                    ;;
                *)
                    echo "Something went wrong"
                    ;;
            esac
        done
        mv "originalGraph.png" $filePath
        mv -vn $csvFile $filePath
        rm -f $parametersFile
    done
    echo "---------------------------------------------"
    echo "--------- FINISHED ALL EXPERIMENTS ----------"
    echo "---------------------------------------------"
}

k=10
m=34
maxN=12
s=200
p=0.5
graphType="gnm"
basePath="../test/FixedNIncreasingK/Gnm"
mkdir -p $basePath
destinyFilePath=$basePath"/Graph3-k$k-m$m-s$s"

mkdir -p $destinyFilePath
mkdir -p $destinyFilePath"/Random"
mkdir -p $destinyFilePath"/RoundRobin"
make
runMultipleExperiments $k $m $p $maxN $s $graphType $order $destinyFilePath"/Graph3-n$n-m$m-s$s"
make clearSpace

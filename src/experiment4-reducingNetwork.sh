#!/bin/bash

runMultipleExperiments() {
    n=$1
    m=$2
    p=$3
    maxK=$4
    s=$5
    graphType=$6
    order=$7
    direction=$8
    filePath=$9
    dynamicsImageFile="./dynamicsGraph.png"
    agentsToAdd=0
    agentsToRemove=0
    modification=""

    # Min-Flow model
    model="min"
    csvFile="$model""Flow-n$n-m$m-s$s.csv"
    touch $csvFile
    echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
    for ((k=1; k<=maxK; k++)) do
        ./dynamics "$n $m $k $s $p $model $graphType $order $modification $agentsToAdd $csvFile"
        mv $dynamicsImageFile $filePath
    done
    rm -f $csvFile

    # Avg-Flow model
    model="avg"
    csvFile="$model""Flow-n$n-m$m-s$s"
    touch $csvFile
    echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
    for ((k=1; k<=maxK; k++)) do
        fileName="avgFlow-$order-k$k.png"
        ./dynamics "$n $m $k $s $p $model $graphType $modification $agentsToAdd $csvFile"
        mv $dynamicsImageFile $filePath
    done
    rm $csvFile
}

n=10
m=8
maxK=12
maxN=1
s=200
agentsToAdd=1
p=0.5
graphType="gnm"
order="ra"
direction=1
experiment="iKfN"
destinyFilePath="../test/randomNetworks/FixedNIncreasingK/Gnm/Graph2-n10-m8-s200/Random"
make
runMultipleExperiments $n $m $p $maxK $s $graphType $order $direction $destinyFilePath

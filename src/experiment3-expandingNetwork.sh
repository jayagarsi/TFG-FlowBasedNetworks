#!/bin/bash

runMultipleExperiments() {
    $n $k $m $p $s $startingAgents $stepSize $graphType $order $destinyFilePath
    n=$1
    k=$2
    m=$3
    p=$4
    s=$5
    startingAgents=$6
    $stepSize=$7
    graphType=$8
    order=$9
    filePath=${10}
    parametersFile="./dynamicsParameters.txt"
    modification="exp"

    for order in $orders
    do
        # Min-Flow model
        model="min"
        csvFile="$model""Flow-$order-n$n-m$m-s$s.csv"
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((agentsToAdd=1; agentsToAdd<=; k++)) do
            echo "--------- EXPERIMENT WITH K $K ---------"
            dynamicsImageFile=$model"Flow-"$order"-k"$k".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > $parametersFile
            ./dynamics
            case $order in
                ra)
                    mv $dynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv $dynamicsImageFile $filePath"/RoundRobin/"
                    ;;
                *)
                    echo "Something went wrong"
                    ;;
            esac
        done
        mv -vn $csvFile $filePath
        rm -f $parametersFile

        # Avg-Flow model
        model="avg"
        case $graphType in
            gnp)
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                ;;
            gnm)
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                ;;
            empty)
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n.csv"
                ;;
            *)
                echo "Something went wrong when assigning csvFile"
                ;;
        esac
        touch $csvFile
        echo "AgentsAdded,NumAgens,OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((k=1; k<=maxK; k++)) do
            echo "--------- EXPERIMENT WITH K $K ---------"
            dynamicsImageFile=$model"Flow-"$order"-k"$k".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > $parametersFile
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
        mv -vn "originalGraph-k$k.png" $filePath
        mv -vn $csvFile $filePath
        rm -f $parametersFile
    done
    echo "---------------------------------------------"
    echo "--------- FINISHED ALL EXPERIMENTS ----------"
    echo "---------------------------------------------"
}

n=10
k=2
m=8
p=0.5
s=200
startingAgents=5
stepSize=5
graphType="gnm"
basePath="../test/NetworkExpansion/Gnm"
mkdir -p $basePath
destinyFilePath=$basePath"/Graph3-n$n-m$m-s$s"

mkdir -p $destinyFilePath
mkdir -p $destinyFilePath"/Random"
mkdir -p $destinyFilePath"/RoundRobin"
make
runMultipleExperiments $n $k $m $p $s $startingAgents $stepSize $graphType $order $destinyFilePath
make clearSpace
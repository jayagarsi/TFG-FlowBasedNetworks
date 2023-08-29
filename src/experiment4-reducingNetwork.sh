#!/bin/bash

runMultipleExperiments() {
    n=$1
    k=$2
    m=$3
    p=$4
    s=$5
    startingAgents=$6
    stepSize=$7
    maximumReduction=$8
    graphType=$9
    filePath=${10}
    parametersFile="./dynamicsParameters.txt"
    modification="red"
    orders="ra pr"
    models="min avg"
    experiment="networkReduction"

    for order in $orders
    do
        for model in $models
        do
            case $graphType in
                gnp)
                    csvFile="$model""Flow-$order-n$n-k$k-p$p-s$s-stepSize$stepSize.csv"
                    ;;
                gnm)
                    csvFile="$model""Flow-$order-n$n-k$k-m$m-s$s-stepSize$stepSize.csv"
                    ;;
                empty)
                    csvFile="$model""Flow-$order-n$n-k$k-stepSize$stepSize.csv"
                    ;;
                *)
                    echo "Something went wrong when assigning csvFile"
                    ;;
            esac
            touch $csvFile
            echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust,chgEquilibriumUtility,chgNrounds,chgNedgesDir,chgNedgesUndir,chgMaxClust" > $csvFile
            for ((agentsToRemove=startingAgents; agentsToRemove<=maximumReduction; agentsToRemove+=stepSize)) do
                echo "--------- EXPERIMENT WHEN REMOVING $agentsToRemove agents ---------"
                originalDynamicsImageFile="originalDynamics-"$order"-"$model"Flow.png"
                reducedDynamicsImageFile="reducedDynamics-"$order"-"$model"Flow-n"$agentsToRemove".png"
                echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo $agentsToRemove > $parametersFile
                if ./dynamics; then
                    echo "Dynamics was executed correctly!"
                else
                    echo "Someting went wrong executing the Dynamics"
                    echo ",,,,,,,,,,," >> $csvFile
                fi
                case $order in
                    ra)
                        mv -vn $originalDynamicsImageFile $filePath
                        mv -vn $reducedDynamicsImageFile $filePath"/Random/"
                        ;;
                    rr)
                        mv -vn $originalDynamicsImageFile $filePath
                        mv -vn $reducedDynamicsImageFile $filePath"/RoundRobin/"
                        ;;
                    pr)
                        mv -vn $originalDynamicsImageFile $filePath
                        mv -vn $reducedDynamicsImageFile $filePath"/PersonalOrder/"
                        ;;
                    *)
                        echo "Something went wrong"
                        ;;
                esac
            done
            mv -vn $csvFile $filePath
            rm -f $parametersFile
        done
        mv -vn "originalGraph.png" $filePath
        mv -vn $csvFile $filePath
        rm -f $parametersFile
    done
    echo "---------------------------------------------"
    echo "--------- FINISHED ALL EXPERIMENTS ----------"
    echo "---------------------------------------------"
}

# PARAMETERS OF THE EXPERIMENT
n=15
k=3
m=50
p=0.5
s=500
startingAgents=1
stepSize=1
maximumReduction=10
graphType="gnp"
case $graphType in
    gnp)
        basePath="../test/NetworkReduction/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-k$k-p$p-s$s"
        ;;
    gnm)
        basePath="../test/NetworkReduction/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-k$k-m$m-s$s"
        ;;
    empty)
        basePath="../test/NetworkReduction/emptyGraph"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-k$k-s$s"
        ;;
    *)
        echo "Pick a correct graph type"
        return
        ;;
esac

mkdir -p $destinyFilePath
mkdir -p $destinyFilePath"/Random"
mkdir -p $destinyFilePath"/RoundRobin"
mkdir -p $destinyFilePath"/PersonalOrder"
make
runMultipleExperiments $n $k $m $p $s $startingAgents $stepSize $maximumReduction $graphType $destinyFilePath
make clearSpace
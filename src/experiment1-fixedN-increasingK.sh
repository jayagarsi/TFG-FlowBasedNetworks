#!/bin/bash

runMultipleExperiments() {
    n=$1
    m=$2
    p=$3
    maxK=$4
    s=$5
    graphType=$6
    filePath=$7
    parametersFile="./dynamicsParameters.txt"
    agentsToAdd=0
    agentsToRemove=0
    modification="none"
    orders="rr ra"
    experiment="fixedNincreasingK"

    for order in $orders
    do
        # Min-Flow model
        model="min"
        case $graphType in
            gnp)
                csvFile="$model""Flow-$order-n$n-p$p.csv"
                ;;
            gnm)
                csvFile="$model""Flow-$order-n$n-m$m-s$s.csv"
                ;;
            empty)
                csvFile="$model""Flow-$order-n$n.csv"
                ;;
            *)
                echo "Something went wrong when assigning csvFile"
                ;;
        esac
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((k=1; k<=maxK; k++)) do
            echo "--------- EXPERIMENT WITH K $K ---------"
            dynamicsImageFile=$model"Flow-"$order"-k"$k".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,," > $csvFile
            fi
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
            mv -vn "originalGraph-k$k.png" $filePath
        done
        mv -vn $csvFile $filePath
        rm -f $parametersFile

        # Avg-Flow model
        model="avg"
        case $graphType in
            gnp)
                csvFile="$model""Flow-$order-n$n-p$p.csv"
                ;;
            gnm)
                csvFile="$model""Flow-$order-n$n-m$m-s$s.csv"
                ;;
            empty)
                csvFile="$model""Flow-$order-n$n.csv"
                ;;
            *)
                echo "Something went wrong when assigning csvFile"
                ;;
        esac
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust" > $csvFile
        for ((k=1; k<=maxK; k++)) do
            echo "--------- EXPERIMENT WITH K $K ---------"
            dynamicsImageFile=$model"Flow-"$order"-k"$k".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,," > $csvFile
            fi
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
    done
    echo "---------------------------------------------"
    echo "--------- FINISHED ALL EXPERIMENTS ----------"
    echo "---------------------------------------------"
}

n=5
maxK=10
m=34
p=0.75
s=200
graphType="empty"
case $graphType in
    gnp)
        basePath="../test/FixedNIncreasingK/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-p$p-s$s"
        ;;
    gnm)
        basePath="../test/FixedNIncreasingK/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-m$m-s$s"
        ;;
    empty)
        basePath="../test/FixedNIncreasingK/emptyGraph"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-s$s"
        ;;
    *)
        echo "Pick a correct graph type"
        return
        ;;
esac

mkdir -p $destinyFilePath
mkdir -p $destinyFilePath"/Random"
mkdir -p $destinyFilePath"/RoundRobin"
make
runMultipleExperiments $n $m $p $maxK $s $graphType $destinyFilePath
make clearSpace

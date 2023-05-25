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
    experiment="fixedKincreasingN"

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
        for ((n=3; n<=maxN; n++)) do
            echo "--------- EXPERIMENT WITH N $N ---------"
            dynamicsImageFile=$model"Flow-"$order"-n"$n".png"
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
            mv -vn "originalGraph-n$n.png" $filePath
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
        for ((n=3; n<=maxN; n++)) do
            echo "--------- EXPERIMENT WITH N $N ---------"
            dynamicsImageFile=$model"Flow-"$order"-n"$n".png"
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

k=5
m=70
maxN=12
s=200
p=0.75
graphType="gnp"
case $graphType in
    gnp)
        basePath="../test/FixedKIncreasingN/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph1-k$k-p$p-s$s"
        ;;
    gnm)
        basePath="../test/FixedKIncreasingN/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph1-k$k-m$m-s$s"
        ;;
    empty)
        basePath="../test/FixedKIncreasingN/emptyGraph"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph1-k$k-s$s"
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
runMultipleExperiments $k $m $p $maxN $s $graphType $destinyFilePath
make clearSpace
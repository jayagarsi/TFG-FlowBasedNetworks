#!/bin/bash

runMultipleExperiments() {
    n=$1
    k=$2
    m=$3
    p=$4
    s=$5
    startingAgents=$6
    stepSize=$7
    maximumExpansion=$8
    graphType=$9
    filePath=${10}
    parametersFile="./dynamicsParameters.txt"
    modification="exp"
    orders="ra pr"
    experiment="networkExpansion"

    for order in $orders
    do
        # Min-Flow model
        model="min"
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
        for ((agentsToAdd=startingAgents; agentsToAdd<=maximumExpansion; agentsToAdd+=stepSize)) do
            echo "--------- EXPERIMENT WHEN ADDING $agentsToAdd agents ---------"
            originalDynamicsImageFile="originalDynamics-"$order"-"$model"Flow.png"
            expandedDynamicsImageFile="expandedDynamics-"$order"-"$model"Flow-n"$agentsToAdd".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo $agentsToAdd > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,,,,,,,," >> $csvFile
            fi
            case $order in
                ra)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/RoundRobin/"
                    ;;
                pr)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/PersonalOrder/"
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
        for ((agentsToAdd=startingAgents; agentsToAdd<=maximumExpansion; agentsToAdd+=stepSize)) do
            echo "--------- EXPERIMENT WHEN ADDING $agentsToAdd agents ---------"
            originalDynamicsImageFile="originalDynamics-"$order"-"$model"Flow.png"
            expandedDynamicsImageFile="expandedDynamics-"$order"-"$model"Flow-n"$agentsToAdd".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo $agentsToAdd  > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,,,,,,,," >> $csvFile
            fi
            case $order in
                ra)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/RoundRobin/"
                    ;;
                pr)
                    mv -vn $originalDynamicsImageFile $filePath
                    mv -vn $expandedDynamicsImageFile $filePath"/PersonalOrder/"
                    ;;
                *)
                    echo "Something went wrong"
                    ;;
            esac
        done
        mv -vn "originalGraph.png" $filePath
        mv -vn $csvFile $filePath
        rm -f $parametersFile
    done
    echo "---------------------------------------------"
    echo "--------- FINISHED ALL EXPERIMENTS ----------"
    echo "---------------------------------------------"
}

n=5
k=3
m=50
p=0.5
s=500
startingAgents=5
stepSize=5
maximumExpansion=15
graphType="gnp"
case $graphType in
    gnp)
        basePath="../test/NetworkExpansion/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-k$k-p$p-s$s"
        ;;
    gnm)
        basePath="../test/NetworkExpansion/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-k$k-m$m-s$s"
        ;;
    empty)
        basePath="../test/NetworkExpansion/emptyGraph"
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
runMultipleExperiments $n $k $m $p $s $startingAgents $stepSize $maximumExpansion $graphType $destinyFilePath
make clearSpace
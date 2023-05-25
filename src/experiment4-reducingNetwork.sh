#!/bin/bash

runMultipleExperiments() {
    $n $k $m $p $s $startingAgents $stepSize $maximumReduction $graphType $destinyFilePath
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
    modification="exp"
    orders="ra pr"

    for order in $orders
    do
        # Min-Flow model
        model="min"
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
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust,chgOriginalUtility,chgEquilibriumUtility,chgNrounds,chgNedgesDir,chgNedgesUndir,chgMaxClust" > $csvFile
        for ((agentsToRemove=startingAgents; agentsToRemove<=maximumReduction; agentsToRemove-=stepSize)) do
            echo "--------- EXPERIMENT WHEN REMOVING $agentsToRemove agents ---------"
            originalDynamicsImageFile="originalDynamics-"$prder"-"$model"Flow"
            reducedDynamicsImageFile="reducedDynamics-"$order"-"$model"Flow-n"$agentsToAdd
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo > $agentsToRemove > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,," > $csvFile
            fi
            case $order in
                ra)
                    mv $originalDynamicsImageFile $filePath
                    mv $reducedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv $originalDynamicsImageFile $filePath
                    mv $reducedDynamicsImageFile $filePath"/RoundRobin/"
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
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust,chgOriginalUtility,chgEquilibriumUtility,chgNrounds,chgNedgesDir,chgNedgesUndir,chgMaxClust" > $csvFile
        for ((agentsToRemove=startingAgents; agentsToRemove<=maximumExpansion; agentsToRemove-=stepSize)) do
            echo "--------- EXPERIMENT WHEN REMOVING $agentsToRemove agents ---------"
            originalDynamicsImageFile="originalDynamics-"$prder"-"$model"Flow"
            reducedDynamicsImageFile="reducedDynamics-"$order"-"$model"Flow-n"$agentsToAdd
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo > $agentsToAdd  > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,," > $csvFile
            fi
            case $order in
                ra)
                    mv $originalDynamicsImageFile $filePath
                    mv $reducedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv $originalDynamicsImageFile $filePath
                    mv $reducedDynamicsImageFile $filePath"/RoundRobin/"
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

n=10
k=2
m=8
p=0.5
s=200
startingAgents=5
stepSize=5
maximumReduction=25
graphType="gnm"
case $graphType in
    gnp)
        basePath="../test/NetworkReduction/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-p$p-s$s"
        ;;
    gnm)
        basePath="../test/NetworkReduction/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-m$m-s$s"
        ;;
    empty)
        basePath="../test/NetworkReduction/emptyGraph"
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
runMultipleExperiments $n $k $m $p $s $startingAgents $stepSize $maximumReduction $graphType $destinyFilePath
make clearSpace
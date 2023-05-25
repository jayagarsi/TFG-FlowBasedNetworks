#!/bin/bash

runMultipleExperiments() {
    $n $k $m $p $s $startingAgents $stepSize $maximumExpansion $graphType $destinyFilePath
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

    for order in $orders
    do
        # Min-Flow model
        model="min"
        case $graphType in
            gnp)
                csvFile="networkExpansion-$model""Flow-$order-n$n-p$p-s$s.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                ;;
            gnm)
                csvFile="networkExpansion-$model""Flow-$order-n$n-m$m-s$s.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                ;;
            empty)
                csvFile="networkExpansion-$model""Flow-$order-n$n.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n.csv"
                ;;
            *)
                echo "Something went wrong when assigning csvFile"
                ;;
        esac
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust,chgOriginalUtility,chgEquilibriumUtility,chgNrounds,chgNedgesDir,chgNedgesUndir,chgMaxClust" > $csvFile
        for ((agentsToAdd=startingAgents; agentsToAdd<=maximumExpansion; agentsToAdd+=stepSize)) do
            echo "--------- EXPERIMENT WHEN ADDING $agentsToAdd agents ---------"
            originalDynamicsImageFile="originalDynamics-"$order"-"$model"Flow.png"
            expandedDynamicsImageFile="expandedDynamics-"$order"-"$model"Flow-n"$agentsToAdd".png"
            echo $n > echo $k > echo $m > echo $p > echo $s > echo $model > echo $graphType > echo $order > echo $modification > echo $csvFile > echo $experiment > echo > $agentsToAdd > $parametersFile
            if ./dynamics; then
                echo "Dynamics was executed correctly!"
            else
                echo "Someting went wrong executing the Dynamics"
                echo ",,,,," > $csvFile
            fi
            case $order in
                ra)
                    mv $originalDynamicsImageFile $filePath
                    mv $expandedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv $originalDynamicsImageFile $filePath
                    mv $expandedDynamicsImageFile $filePath"/RoundRobin/"
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
                csvFile="networkExpansion-$model""Flow-$order-n$n-p$p-s$s.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-p$p-s$s.csv"
                ;;
            gnm)
                csvFile="networkExpansion-$model""Flow-$order-n$n-m$m-s$s.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n-m$m-s$s.csv"
                ;;
            empty)
                csvFile="networkExpansion-$model""Flow-$order-n$n.csv"
                csvFileOriginalDynamics="originalDynamics-$model""Flow-$order-n$n.csv"
                csvFileExpandedDynamics="expandedDynamics-$model""Flow-$order-n$n.csv"
                ;;
            *)
                echo "Something went wrong when assigning csvFile"
                ;;
        esac
        touch $csvFile
        echo "OriginalUtility,EquilibriumUtility,Nrounds,NedgesDir,NedgesUndir,MaxClust,chgOriginalUtility,chgEquilibriumUtility,chgNrounds,chgNedgesDir,chgNedgesUndir,chgMaxClust" > $csvFile
        for ((agentsToAdd=startingAgents; agentsToAdd<=maximumExpansion; agentsToAdd+=stepSize)) do
            echo "--------- EXPERIMENT WHEN ADDING $agentsToAdd agents ---------"
            originalDynamicsImageFile="originalDynamics-"$order"-"$model"Flow.png"
            expandedDynamicsImageFile="expandedDynamics-"$order"-"$model"Flow-n"$agentsToAdd".png"
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
                    mv $expandedDynamicsImageFile $filePath"/Random/"
                    ;;
                rr)
                    mv $originalDynamicsImageFile $filePath
                    mv $expandedDynamicsImageFile $filePath"/RoundRobin/"
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
maximumExpansion=25
graphType="empty"
case $graphType in
    gnp)
        basePath="../test/NetworkExpansion/Gnp"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-p$p-s$s"
        ;;
    gnm)
        basePath="../test/NetworkExpansion/Gnm"
        mkdir -p $basePath
        destinyFilePath=$basePath"/Graph2-n$n-m$m-s$s"
        ;;
    empty)
        basePath="../test/NetworkExpansion/emptyGraph"
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
runMultipleExperiments $n $k $m $p $s $startingAgents $stepSize $maximumExpansion $graphType $destinyFilePath
make clearSpace
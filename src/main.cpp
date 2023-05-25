#include "Network.hh"
#include <string>
using namespace std;

void printCapacityVector(Network& G, int direction) {
    int totalEdges = 0;
    vector<int> edges = G.numberOfEdges(direction);
    cout << "Number of edges in the " << (direction ? "undirected" : "directed") << " network:" << endl;
    for (int i = 0; i < edges.size(); i++) {
        totalEdges += edges[i];
        cout << "     Capacity " << i+1 << ": " << edges[i] << endl;
    }
    cout << "     ------------------" << endl;
    cout << "     Total capacity: " << totalEdges << endl;
    cout << endl;
}

void printMaximalCluster(Network& G, int k) {
    vector<int> maximalCluster = G.computeMaximalCluster(k+1);
    cout << "One possible maximal cluster of size " << maximalCluster.size() << ":" << endl;
    for (int i = 0; i < maximalCluster.size(); ++i) cout << maximalCluster[i] << ' ';
    cout << endl;
}

int main() {
    // Parameters to the simulation
    
    int n = 10;
    int m = 34;
    int k = 6;
    int s = 200;
    int agentsToAdd = 30;
    int agentsToRemove = 17;
    double p = 0.75;
    string model = "min";
    string graphType = "gnp";
    string modification = "none";            // red == reeduction, exp == expansion, none
    string order = "rr";                    // rr === Round Robin, ra === Random, pr === Personal
    int direction = 1;                      // 0  === directed, 1 === undirected
    int numRounds = 0;
    
    Network G(n, k);
   
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------ ORIGINAL GRAPH --------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    if (graphType == "gnm" or graphType == "gnp") 
        G.buildRandomGraph(n, m, k, s, p, graphType);

    G.drawGraph(direction, "originalGraph");
    G.printModelsUtility(model);
    printCapacityVector(G, direction);
    
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "----------- ORIGINAL DYNAMICS ------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    if (order == "ra") numRounds = G.simulateGameDynamicsRandomOrder(model, s);
    else if (order == "rr") numRounds = G.simulateGameDynamics(model);
    else if (order == "pr") {
        vector<int> agentOrder(n);
        for (int i = 0; i < n; ++i)
            agentOrder[i] = n-i-1;
        numRounds = G.simulateGameDynamics(model, agentOrder);
    }
    string name = "originalDynamics-" + order + "-" + model + "Flow";
    G.drawGraph(direction, name);
    G.printModelsUtility(model);
    cout << "Number of rounds played: " << numRounds << endl;
    printCapacityVector(G, 0);
    printCapacityVector(G, 1);
    printMaximalCluster(G, k+1);

    if (modification == "exp") {
        cout << "------------------------------------------" << endl;
        cout << "------------------------------------------" << endl;
        cout << "-------- EXPANDED GRAPH DYNAMICS ---------" << endl;
        cout << "------------------------------------------" << endl;
        cout << "------------------------------------------" << endl;
        n = n + agentsToAdd;
        G.addNewAgents(agentsToAdd);
        if (order == "ra") numRounds = G.simulateGameDynamicsRandomOrder(model, s);
        else if (order == "rr") numRounds = G.simulateGameDynamics(model);
        else if (order == "pr") {
            vector<int> agentOrder(n);
            for (int i = 0; i < n; ++i)
                agentOrder[i] = n-i-1;
            numRounds = G.simulateGameDynamics(model, agentOrder);
        }
        name = "expandedDynamics-" + order + "-" + model + "Flow-n" + to_string(agentsToAdd);  
        G.drawGraph(direction, name);
        G.printModelsUtility(model);
        cout << "Number of rounds played: " << numRounds << endl;
        printCapacityVector(G, 0);
        printCapacityVector(G, 1);
        printMaximalCluster(G, k+1);
    }

    else if (modification == "red") {
        cout << "------------------------------------------" << endl;
        cout << "------------------------------------------" << endl;
        cout << "--------- REDUCED GRAPH DYNAMICS ---------" << endl;
        cout << "------------------------------------------" << endl;
        cout << "------------------------------------------" << endl;
        n = n - agentsToRemove;
        G.removeAgents(agentsToRemove);
        if (order == "ra") numRounds = G.simulateGameDynamicsRandomOrder(model, s);
        else if (order == "rr") numRounds = G.simulateGameDynamics(model);
        else if (order == "pr") {
            vector<int> agentOrder(n);
            for (int i = 0; i < n; ++i)
                agentOrder[i] = n-i-1;
            numRounds = G.simulateGameDynamics(model, agentOrder);
        }

        name = "reducedDynamics-" + order + "-" + model + "Flow-n" + to_string(agentsToRemove);  
        G.drawGraph(direction, name);
        G.printModelsUtility(model);
        cout << "Number of rounds played: " << numRounds << endl;
        printCapacityVector(G, 0);
        printCapacityVector(G, 1);
        printMaximalCluster(G, k+1);
    }
}


    /*G.buyEdge(0, 1, 2);
    G.buyEdge(1, 2, 2);
    G.buyEdge(2, 3, 2);
    G.buyEdge(3, 4, 2);
    G.buyEdge(4, 1, 2);
    
    G.simulateGameDynamicsRandomOrder(model);
    G.drawGraph(0, "equilibriumGraphRand2");*/

    
    /*G.buildRandomGraph(n, m, k, s, p, graphType);
    G.computeAndApplyAgentBestResponse(agent, model);
    */
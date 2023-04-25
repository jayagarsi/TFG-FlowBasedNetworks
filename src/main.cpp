#include "Network.hh"
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
    int m = 25;
    int k = 12;
    int s = 200;
    int agent = 0;
    int agentsToAdd = 1;
    double p = 0.5;
    string model = "avg";
    string graphType = "erdos";
    int direction = 1;              // 0 === directed, 1 === undirected

    Network G(n, k);
   
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------ ORIGINAL GRAPH --------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    // G.buildRandomGraph(n, m, k, s, p, graphType);
    
    // printCapacityVector(G, 0);
    // printCapacityVector(G, 1);
    G.drawGraph(direction, "originalGraph");
    
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "----------- ORIGINAL DYNAMICS ------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    G.simulateGameDynamicsRandomOrder(model, s);
    // G.simulateGameDynamics(model);
    G.drawGraph(direction, "dynamicsGraph");
    printCapacityVector(G, 0);
    printCapacityVector(G, 1);
    printMaximalCluster(G, k);

    // UNCOMMENT FOR EXPANDING THE NETWORK WHEN IN NE
    /*
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "-------- EXPANDED GRAPH DYNAMICS ---------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    G.addNewAgents(agentsToAdd);
    G.simulateGameDynamicsRandomOrder(model, s);
    G.drawGraph(direction, "expandedDynamicsGraph");
    edgeCapacity = G.numberOfEdges(direction);
    printCapacityVector(edgeCapacity, direction);
    */

    // UNCOMMENT FOR REDUCING THE NETWORK WHEN IN NE
   /*
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "--------- REDUCED GRAPH DYNAMICS ---------" << endl;
    cout << "------------------------------------------" << endl;
    cout << "------------------------------------------" << endl;
    G.addNewAgents(agentsToAdd);
    G.simulateGameDynamicsRandomOrder(model, s);
    G.drawGraph(direction, "expandedDynamicsGraph");
    edgeCapacity = G.numberOfEdges(direction);
    printCapacityVector(edgeCapacity, direction);
    */
    
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
#include "Network.hh"
using namespace std;

int main() {
    int n = 4;
    int k = 3;
    Network G(n, k);
    G.buyEdge(0, 1, 2);
    G.buyEdge(0, 3, 1);
    G.buyEdge(1, 0, 1);
    G.buyEdge(1, 2, 2);
    G.buyEdge(2, 0, 3);
    G.buyEdge(3, 0, 3);

    //G.computeAllGameCombinations();
    /*G.printAdjacencyMatrix(0);
    cout << endl;
    */
    G.printAdjacencyMatrix(0);

    G.printAllModelsUtility();
    int a = 2;
    vector<int> BR = G.agentBestResponse(a, "min");
    for (int i = 0; i < n; ++i) {
        cout << "(" << a << ", " << i << "): " << BR[i] << endl;
    }
    G.setAgentStrategy(a, BR);
    G.printAllModelsUtility();
    G.printAdjacencyMatrix(0);
    
}

// Amb nodes "fulla" sembla que la millor estrategia per l'agent
// es gastar-ho tot en un sol agent (cal corroborar-ho)
#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 7;
    int k = 2;
    Network G(n, k);
    string model = "min";
    G.buyEdge(0, 1, 2);
    G.buyEdge(1, 2, 1);
    G.buyEdge(1, 3, 1);
    G.buyEdge(2, 0, 1);
    G.buyEdge(2, 5, 1);
    G.buyEdge(3, 4, 2);
    G.buyEdge(4, 5, 2);
    G.buyEdge(5, 6, 2);
    G.buyEdge(6, 3, 2);

    //G.simulateGameDynamics(model);
    G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);
    vector<int> bestStrategy = G.agentBestResponse(0, model);
    /*for (int i = 0; i < bestStrategy.size(); ++i)
        cout << bestStrategy[i] << endl;*/
    G.setAgentStrategy(0, bestStrategy);
    G.printModelsUtility(model);
    G.printAdjacencyMatrix(0);
    
}

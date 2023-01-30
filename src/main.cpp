#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 6;
    int k = 2;
    Network G(n, k);
    string model = "min";
    G.buyEdge(0, 1, 2);
    G.buyEdge(1, 0, 1);
    G.buyEdge(1, 3, 1);
    G.buyEdge(2, 1, 2);
    G.buyEdge(3, 4, 2);
    G.buyEdge(4, 2, 1);
    G.buyEdge(4, 3, 1);
    G.buyEdge(5, 4, 2);

    G.simulateGameDynamics(model);
    /*G.printAdjacencyMatrix(0);
    G.printModelsUtility("min");
    vector<int> bestStrategy = G.agentBestResponse(0, model);
    /*for (int i = 0; i < bestStrategy.size(); ++i)
        cout << bestStrategy[i] << endl;
    G.setAgentStrategy(0, bestStrategy);
    G.printModelsUtility("min");
    G.printAdjacencyMatrix(0);
    */
}

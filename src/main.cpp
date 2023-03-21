#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 4;
    int k = 6;
    // Vertex a == 0, b == 1, c == 2, d == 3, f == 4, g == 5
    Network G(n, k);
    string model = "min";

    //G.drawGraph(0);
    G.simulateGameDynamicsRandomOrder(model);

    //int v[] = {4, 1, 0, 2, 3, 5};
    //vector<int> order(v, v+sizeof(v)/sizeof(int));
    //G.simulateGameDynamics(model, order);
    /*G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);
    vector<int> bestStrategy = G.agentBestResponse(0, model);
    /*for (int i = 0; i < bestStrategy.size(); ++i)
        cout << bestStrategy[i] << endl;
    G.setAgentStrategy(0, bestStrategy);
    G.printModelsUtility(model);
    G.printAdjacencyMatrix(0);
    cout << endl;
    */

    G.drawGraph(0);
}

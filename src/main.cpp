#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 100;
    int k = 1;
    // Vertex a == 0, b == 1, c == 2, d == 3, f == 4, g == 5
    Network G(n, k);
    string model = "avg";
    string graphType = "sparse";
    G.buildRandomGraph(n, n, k, 200, 0, graphType);
    G.drawGraph(0, "originalGraph");
    G.simulateGameDynamics(model);
    //G.simulateGameDynamicsRandomOrder(model);
    G.drawGraph(0, "dynamicsGraph");


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
}

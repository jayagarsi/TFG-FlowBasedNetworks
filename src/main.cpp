#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 7;
    int k = 2;
    // Vertex a == 0, b == 1, c == 2, d == 3, f == 4, g == 5
    Network G(n, k);
    string model = "avg";
    G.buyEdge(0, 1, 2);
    G.buyEdge(1, 2, 1);
    G.buyEdge(1, 3, 1);
    G.buyEdge(2, 0, 1);
    G.buyEdge(2, 5, 1);
    G.buyEdge(3, 4, 2);
    G.buyEdge(4, 5, 2);
    G.buyEdge(5, 6, 2);
    G.buyEdge(6, 3, 2);
    //G.drawGraph(0);
    G.simulateGameDynamics(model);

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

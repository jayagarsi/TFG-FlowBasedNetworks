#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 4;
    int k = 2;
    Network G(n, k);
    // Vertex 0 == v, 1 == x, 2 == y, z == 3
    string model = "min";
    G.buyEdge(0, 1, 1);
    G.buyEdge(0, 3, 1);
    G.buyEdge(1, 0, 1);
    G.buyEdge(1, 3, 1);
    G.buyEdge(2, 1, 1);
    G.buyEdge(2, 3, 1);
    G.buyEdge(3, 2, 2); 
    G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);
    cout << endl << endl;

    //vector<int> agentStrategy = G.agentBestResponse(0, model);
    G.simulateGameDynamics(model);

    //vector<int> bestStrategy = G.agentBestResponse(0, model);
    //for (int i = 0; i < bestStrategy.size(); ++i)
    //    cout << bestStrategy[i] << endl;
    G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);
}

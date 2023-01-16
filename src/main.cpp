#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 4;
    int k = 3;
    Network G(n, k);
    string model = "min";
    // Vertex a == 0, b == 1, c == 2, d == 3, e == 4, 
    G.buyEdge(0, 1, 2);
    G.buyEdge(0, 3, 1);
    G.buyEdge(1, 0, 1);
    G.buyEdge(1, 2, 2);
    G.buyEdge(2, 0, 3);
    G.buyEdge(3, 0, 3);
    cout << "Original Graph" << endl;
    G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);
    
    //vector<int> agentStrategy = G.agentBestResponse(0, model);
    G.simulateGameDynamics(model);
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    G.printAdjacencyMatrix(0);
    G.printModelsUtility(model);

    /*
    vector<int> bestStrategy = G.agentBestResponse(0, model);
    for (int i = 0; i < bestStrategy.size(); ++i)
        cout << bestStrategy[i] << endl;
    */
}

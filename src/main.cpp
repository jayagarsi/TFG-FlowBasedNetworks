#include "Network.hh"
#include <fstream>
using namespace std;

int main() {
    int n = 35;
    int m = 20;
    int k = 2;
    int s = 200;
    int agent = 0;
    double p = 0.50;
    string model = "min";
    string graphType = "erdos";
    // Vertex a == 0, b == 1, c == 2, d == 3, f == 4, g == 5
    Network G(n, k);
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
    /*
    G.buildRandomGraph(n, m, k, s, p, graphType);
    G.drawGraph(0, "originalGraph");
    G.simulateGameDynamics(model);
    G.drawGraph(0, "dynamicsGraph");
    */
    //G.simulateGameDynamicsRandomOrder(model);
    // G.drawGraph(0, "dynamicsGraph");
    // cout << (G.isCycleOptimumGraph() ? "The graph generated is an optimum cycle" : "The graph is not a cycle") << endl;
}

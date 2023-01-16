#ifndef NETWORK_HH
#define NETWORK_HH

#include <iostream>
#include <vector>
using namespace std;

using Graph = vector<vector<int>>;
const int INF = 1e9;

class Network {

    private:
        int n, k = 0;
        vector<int> budgets;
        Graph G;        // directed  version
        Graph F;        // undireced version
        double networkMinCut = 0.0;

        // Auxiliars
        void eraseAllConnections(Graph& G);
        bool BFS(const Graph& residualG, int s, int t, vector<int>& path, int& minPathCapacity);
        double minimumSTCut(Graph& F, int s, int t);        
        void DFS(const Graph& residualG, int s, vector<bool>& isInMinimumCut);
        void bestResponseMinFlow(Graph& GR, int u, int kUsed, pair<int, int>& maxUtility, vector<int>& maxStrategy);
        void bestResponseAvgFlow(Graph& GR, int u, int kUsed, double& maxUtility, vector<int>& maxStrategy);

    public:
        // Constructors
        Network();
        Network(int nAgents, int budget);
        ~Network();

        // Random Generators
        void buildRandomGraph();
        
        // Strategy Changes
        void buyEdge(int u, int v, int w);
        void sellEdge(int u, int v, int w);
        
        // Exhaustive Search
        vector<int> agentBestResponse(int u, const string& model);
        bool isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model);

        // Auxiliars
        double minimumGraphCut(Graph& F);
        void printAdjacencyMatrix(int g);
        void printAdjacencyMatrix(const Graph& G);
        void printModelsUtility(const string& model);
        bool setAgentStrategy(int u, const vector<int>& st);
        void simulateGameDynamics(const string& model);
        void convertDirectedToUndirected(Graph& G, Graph& F);

        // AVG-FLOW Model
        double avgFlowAgentUtility(Graph& F, int u);
        double avgFlowSocialUtility(Graph& F);

        // MIN-FLOW Model
        int wellConnectedNeighbours(Graph& F, int u);
        pair<int, int> minFlowAgentUtility(Graph& F, int u);
        double minFlowSocialUtility(Graph& F);

};

#endif
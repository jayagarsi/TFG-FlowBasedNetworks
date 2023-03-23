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

        // Flow Computations
        bool BFS(const Graph& residualG, int s, int t, vector<int>& path, int& minPathCapacity);      
        void DFS(const Graph& residualG, int s, vector<bool>& isInMinimumCut);
        double minimumSTCut(Graph& F, int s, int t);

        // Best Response Models
        void eraseAllConnections(Graph& G);
        void bestResponseMinFlow(Graph& GR, int u, int kUsed, pair<int, int>& maxUtility, vector<int>& maxStrategy);
        void bestResponseAvgFlow(Graph& GR, int u, int kUsed, double& maxUtility, vector<int>& maxStrategy);

    public:
        // Constructors
        Network();
        Network(int nAgents, int budget);
        ~Network();
    
        // Random Generators
        void buildRandomGraph(int n, int m, int k, int s, double p, const string& type);
        
        // Strategy Changes
        void buyEdge(int u, int v, int w);
        void sellEdge(int u, int v, int w);
        bool setAgentStrategy(int u, const vector<int>& st);

        // Network Computations
        double minimumGraphCut(Graph& F);
        int agentDegree(Graph& F, int u);
        void convertDirectedToUndirected(Graph& G, Graph& F);
        
        // Exhaustive Search
        vector<int> agentBestResponse(int u, const string& model);
        vector<int> agentBestResponseMinFlowDeterministic(int u);
        vector<int> agentBestResponseAvgFlowDeterministic(int u);
        bool isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model);

        // Game Dynamics
        void simulateGameDynamics(const string& model);
        void simulateGameDynamics(const string& model, const vector<int>& agentOrder);
        void simulateGameDynamicsRandomOrder(const string& model);

        // AVG-FLOW Model
        double avgFlowAgentUtility(Graph& F, int u);
        double avgFlowSocialUtility(Graph& F);

        // MIN-FLOW Model
        int wellConnectedNeighbours(Graph& F, int u);
        pair<int, int> minFlowAgentUtility(Graph& F, int u);
        double minFlowSocialUtility(Graph& F);

        // Input/Output
        void printAdjacencyMatrix(int g);
        void printAdjacencyMatrix(const Graph& G);
        void printModelsUtility(const string& model);
        void drawGraph(int g, const string& filename);

        // Auxiliar
        bool isCycleOptimumGraph();

};

#endif
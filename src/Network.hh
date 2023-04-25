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
        void DFS(const Graph& residualG, int n, int s, vector<bool>& isInMinimumCut);
        int minimumSTCut(Graph& F, vector<pair<int, int>> minCutNodes, int s, int t);

        // Best Response Models
        void eraseAllConnections(Graph& G);
        void bestResponseMinFlow(Graph& GR, int u, int kUsed, int lastVisited, pair<int, int>& maxUtility, vector<int>& maxStrategy);
        void bestResponseAvgFlow(Graph& GR, int u, int kUsed, int lastVisited, double& maxUtility, vector<int>& maxStrategy);

        // Auxiliar
        void merge(vector<int>& v, int ini, int mid, int end);
        void mergeSortByDegree(vector<int>& v, int ini, int end);
        void shuffleArray(vector<int>& array);

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
        void addNewAgents(int na);
        void removeAgents(int na);

        // Network Computations
        int minimumGraphCut(Graph& F);
        int agentDegree(Graph& F, int u);
        int agentDegree(int g, int u);
        void convertDirectedToUndirected(Graph& G, Graph& F);
        vector<int> computeMaximalCluster(int j);
        void inducedSubgraph(const Graph& F, Graph& H, vector<int>& inducingNodes, int g);
        
        // Best Response
        vector<int> agentBestResponse(int u, const string& model);
        void computeAndApplyAgentBestResponse(int u, const string& model);
        bool isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model);
        vector<int> agentBestResponseMinFlowDeterministic(int u);
        vector<int> agentBestResponseAvgFlowDeterministic(int u);
        
        // Game Dynamics
        void simulateGameDynamics(const string& model);
        void simulateGameDynamics(const string& model, const vector<int>& agentOrder);
        void simulateGameDynamicsRandomOrder(const string& model, int seed);

        // AVG-FLOW Model
        double avgFlowAgentUtility(Graph& F, int u);
        double avgFlowSocialUtility(Graph& F);

        // MIN-FLOW Model
        int wellConnectedNeighbours(Graph& F, int u, int minCut);
        pair<int, int> minFlowAgentUtility(Graph& F, int u);
        int minFlowSocialUtility(Graph& F);

        // Input/Output
        void printAdjacencyMatrix(int g);
        void printAdjacencyMatrix(const Graph& G);
        void printModelsUtility(const string& model);
        void printModelsUtility(Graph& F, const string& model);
        void drawGraph(int g, const string& filename);
        void readGraphFromFile(const string& filename);
        void writeGraphToFile(int g, const string& filename);

        // Auxiliar
        bool isCycleOptimumGraph();
        vector<int> numberOfEdges(int g);
};

#endif
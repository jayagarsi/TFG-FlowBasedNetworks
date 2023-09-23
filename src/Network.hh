#ifndef NETWORK_HH
#define NETWORK_HH

#include <iostream>
#include <vector>
using namespace std;

using Graph = vector<vector<int>>;
const int INF = 1e9;

class Network {

    protected:
        int n;
        Graph G;        // directed  version
        Graph F;        // undirected version
        double networkMinCut = 0.0;

        // Flow Computations
        bool BFS(const Graph& residualG, int s, int t, vector<int>& path, int& minPathCapacity);      
        void DFS(const Graph& residualG, int n, int s, vector<bool>& isInMinimumCut);
        int minimumSTCut(Graph& F, vector<pair<int, int>> minCutNodes, int s, int t);

        // Auxiliar
        void merge(vector<int>& v, int ini, int mid, int end);
        void mergeSortByDegree(vector<int>& v, int ini, int end);
        void shuffleArray(vector<int>& array);
        void eraseAllConnections(Graph& G);
        void copyGraph(Graph& F, Graph& H);
        void inducedSubgraph(const Graph& F, Graph& H, vector<int>& inducingNodes, int g);
        int positionOfMaxElement(vector<int>& v);

    public:
        // Constructors
        Network();
        Network(int nAgents);
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
        vector<int> numberOfEdges(int g, int k);
        void convertDirectedToUndirected(Graph& G, Graph& F);

        // AVG-FLOW Model
        double avgFlowAgentUtility(Graph& F, int u);
        double avgFlowSocialUtility(Graph& F);
        double avgFlowSocialUtility();

        // MIN-FLOW Model
        int wellConnectedNeighbours(Graph& F, int u, int minCut);
        pair<int, int> minFlowAgentUtility(Graph& F, int u);
        int minFlowSocialUtility(Graph& F);
        int minFlowSocialUtility();

        // Input/Output
        void printAdjacencyMatrix(int g);
        void printAdjacencyMatrix(const Graph& G);
        void printModelsUtility(const string& model);
        void printModelsUtility(Graph& F, const string& model);
        void drawGraph(int g, const string& filename);
        void readGraphFromFile(const string& filename);
        void writeGraphToFile(int g, const string& filename);
        void writeGraphToFile(Graph& F, const string& filename);
        void appendResultsToFile(const string& filename);

        // Auxiliar
        bool isCycleOptimumGraph();
};

#endif
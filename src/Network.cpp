#include "Network.hh"
#include <queue>
#include <climits>
#include <algorithm>
#include <random>
#include <functional>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

/*----------------------- CONSTRUCTORS -----------------------*/

Network::Network() { 
    G = Graph(0, vector<int>(0, 0));
    F = Graph(0, vector<int>(0, 0));
    networkMinCut = 0;
}

Network::Network(int nAgents, int budget) {
    n = nAgents;
    k = budget;
    budgets = vector<int>(n, k);
    G = Graph(n, vector<int>(n, 0));
    F = Graph(n, vector<int>(n, 0));
    networkMinCut = 0;
}

Network::~Network() { }

/*----------------------- FLOW COMPUTATIONS -----------------------*/

/**
 * @brief Traverses a graph represented by and adjacecny matrix wih the Breadth-First Search algorithm.
 * Besides traversing the graph, the algorithm also tells us if there is a path starting from node s that ends in t. 
 * In path this possible rout from s to t is stored  (if it exists) and in minPathCapacity the value 
 * of the smallest capacity between all edges in the path.
 * 
 * @param residualG matrix to traverse
 * @param s starting node
 * @param t finishing node
 * @param path path between s and t (might be empty)
 * @param minPathCapacity minimum capacity of the path
 * @return true if the path exists
 * @return false otherwise
 */
bool Network::BFS(const Graph& residualG, int s, int t, vector<int>& path, int& minPathCapacity) {
    int n = residualG.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 0; v < n; v++) {
            if (not visited[v] and residualG[u][v] > 0) {
                if (residualG[u][v] < minPathCapacity)
                    minPathCapacity = residualG[u][v];
                if (v == t) {
                    path[v] = u;
                    return true;
                }
                q.push(v);
                path[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
    //return visited[t];
}

/**
 * @brief Traverses a graph represented by and adjacecny matrix wih the Depth-First Search algorithm.
 * The algorithm is tuned to save in an array those nodes that belong to the minimum cut. The array starts
 * from node s
 * 
 * @param residualG input graph
 * @param n number of nodes in the graph
 * @param s startoff node
 * @param isInMinimumCut array of bools that indicate for each node if they are in the minimum cut. It is assumed that it is initialized as all false
 */
void Network::DFS(const Graph& residualG, int n, int s, vector<bool>& isInMinimumCut) {
    isInMinimumCut[s] = true;
    for (int u = 0; u < n; ++u) {
        if (residualG[s][u] and not isInMinimumCut[u])
            DFS(residualG, n, u, isInMinimumCut);
    }
}

/**
 * @brief Computes the minimum cut between two nodes in a graph by using the Edmond-Karp algorithm.
 * It also saves those nodes that form the minimum cut.
 * 
 * @param F input graph
 * @param minCutNodes array of pairs of integers with the nodes that belong to the minimum cut
 * @param s source node
 * @param t sink node
 * @return int value of the minimum cut of the network, which is equal to the maximum flow.
 */
int Network::minimumSTCut(Graph& F, vector<pair<int, int>> minCutNodes, int s, int t) {
    int n = F.size();
    Graph residualG(n, vector<int>(n));
    for (int u = 0; u < n; ++u)
        for (int v = 0; v < n; ++v)
            residualG[u][v] = F[u][v];

    vector<int> path(n);
    //int maxFlow = 0;
    int minPathCapacity = INT_MAX;
    while (BFS(residualG, s, t, path, minPathCapacity)) {
        int pathFlow = INF;
        /*
        // Bottleneck of the s-t path
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            pathFlow = min(pathFlow, residualG[u][v]);
        }
        */
        // Augment the s-t path
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            residualG[u][v] -= minPathCapacity;
            residualG[v][u] += minPathCapacity;
        }
        minPathCapacity = INT_MAX;
       // maxFlow += pathFlow;
    }

    vector<bool> isInMinimumCut(n, false);
    DFS(residualG, residualG.size(), s, isInMinimumCut);

    int minCut = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (isInMinimumCut[i] and not isInMinimumCut[j] and F[i][j] > 0) {
                pair<int, int> edge = make_pair(i, j);
                minCutNodes.push_back(edge);
                minCut += F[i][j];
            }
    return minCut;   
}

/*----------------------- BEST RESPONSE MODELS -----------------------*/

/**
 * @brief Removes all edges from a graph
 * 
 * @param G input graph
 */
void Network::eraseAllConnections(Graph& G) {
    int m = G.size();
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            G[i][j] = 0;
}

/**
 * @brief Algorithm to compute the Best Response of an agent in the MinFlow-NCG model by using
 * an exhaustive search.
 * 
 * @param GR input graph
 * @param u agent
 * @param kUsed budget used until now
 * @param lastVisited last seen node (this is to avoid seeing repeated strategies)
 * @param maxUtility value of the maximum utility found until now for agent u
 * @param maxStrategy strategy that gives the maximum utiliy to agent u
 */
void Network::bestResponseMinFlow(Graph& GR, int u, int kUsed, int lastVisited, pair<int, int>& maxUtility, vector<int>& maxStrategy) {
    if (kUsed == k) {
        Graph FR(n, vector<int>(n, 0));
        convertDirectedToUndirected(GR, FR);
        auto actualUtility = minFlowAgentUtility(FR, u);
        // The min-cut stays the same so let's check the second component
        if (actualUtility.first == maxUtility.first) {
            if (actualUtility.second > maxUtility.second) {
                maxUtility.second = actualUtility.second;
                for (int v = 0; v < n; ++v)
                    maxStrategy[v] = GR[u][v]; 
            }
                /*cout << actualUtility.first <<  " " << actualUtility.second << endl;
                cout << " ";
                for (int i = 0; i < maxStrategy.size(); ++i)
                    cout << i << " ";
                cout << endl;
                cout << "[";
                for (int i = 0; i < maxStrategy.size(); ++i)
                    cout << maxStrategy[i] << " ";
                cout << "]" << endl;
                cout << endl;*/
        }
        // The min-cut has upgraded so we have to copy all values independently of wich they are
        else if (actualUtility.first > maxUtility.first) {
            maxUtility.first = actualUtility.first;
            maxUtility.second = actualUtility.second;
            for (int v = 0; v < n; ++v)
                maxStrategy[v] = GR[u][v];
            /*cout << actualUtility.first <<  " " << actualUtility.second << endl;
            cout << " ";
            for (int i = 0; i < maxStrategy.size(); ++i)
                cout << i << " ";
            cout << endl;
            cout << "[";
            for (int i = 0; i < maxStrategy.size(); ++i)
                cout << maxStrategy[i] << " ";
            cout << "]" << endl;
            cout << endl;*/
        }
    }
    else {
        for (int v = lastVisited; v < n; ++v) {
            if (v != u) {
                GR[u][v] += 1;
                kUsed += 1;
                bestResponseMinFlow(GR, u, kUsed, v, maxUtility, maxStrategy);
                kUsed -= 1;
                GR[u][v] -= 1;
            }
        }
    }
}

/**
 * @brief Algorithm to compute the Best Response of an agent in the AvgFlow-NCG model by using
 * an exhaustive search.
 * 
 * @param GR input graph
 * @param u agent
 * @param kUsed budget used until now
 * @param lastVisited last seen node (this is to avoid seeing repeated strategies)
 * @param maxUtility value of the maximum utility found until now for agent u
 * @param maxStrategy strategy that gives the maximum utiliy to agent u
 */
void Network::bestResponseAvgFlow(Graph& GR, int u, int kUsed, int lastVisited, double& maxUtility, vector<int>& maxStrategy) {
    if (kUsed == k) {
        Graph FR(n, vector<int>(n, 0));
        convertDirectedToUndirected(GR, FR);
        double actualUtility = avgFlowAgentUtility(FR, u);
        if (actualUtility > maxUtility) {
            maxUtility = actualUtility;
            for (int v = 0; v < n; ++v)
            maxStrategy[v] = GR[u][v];
            /*cout << actualUtility << endl;
            cout << " ";
            for (int i = 0; i < maxStrategy.size(); ++i)
                cout << i << " ";
            cout << endl;
            cout << "[";
            for (int i = 0; i < maxStrategy.size(); ++i)
                cout << maxStrategy[i] << " ";
            cout << "]" << endl;
            cout << endl;*/
        }
    }
    else {
        for (int v = lastVisited; v < n; ++v) {
            if (v != u) {
                GR[u][v] += 1;
                kUsed += 1;
                bestResponseAvgFlow(GR, u, kUsed, v, maxUtility, maxStrategy);
                kUsed -= 1;
                GR[u][v] -= 1;
            }
        }
    }
}

/*----------------------- PRIVATE AUXILIAR -----------------------*/

/**
 * @brief Merges a given vector. This is to be used with the MergeSort algorithm
 * only.
 * 
 * @param v input array
 * @param ini starting point
 * @param mid middle point
 * @param end ending point
 */
void Network::merge(vector<int>& v, int ini, int mid, int end) {
    vector<int> temp;
    int n1 = mid - ini + 1;
    int n2 = end - mid;

    vector<int> L(n1);
    vector<int> R(n2);
 
    for (int i = 0; i < n1; i++) {
        L[i] = v[ini + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = v[mid + 1 + j];
    }
 
    int i = 0;
    int j = 0;
    int k = ini;
 
    while (i < n1 and j < n2) {
        if (agentDegree(F, L[i]) <= agentDegree(F, R[j])) {
            v[k] = L[i];
            i++;
        }
        else {
            v[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1) {
        v[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        v[k] = R[j];
        j++;
        k++;
    }
}

/**
 * @brief Algorithm to sort a given vector by the Merge Sort algorithm depending
 * on the degree of the agents in the vector. It is assumed that this vector represent
 * a set of agents from a graph G
 * 
 * @param v vector to sort
 * @param ini starting point
 * @param end ending point
 */
void Network::mergeSortByDegree(vector<int>& v, int ini, int end) {
    if (ini < end) {
        int mid = (ini + end)/2;
        mergeSortByDegree(v, ini, mid);
        mergeSortByDegree(v, mid+1, end);
        merge(v, ini, mid, end);
    }
}

/**
 * @brief Shuffles a given array randomly with the Fisher-Yates algorithm.
 * Every agent has the same probability of being picked at each step
 * 
 * @param order array to be randomly shuffled
 */
void Network::shuffleArray(vector<int>& order) {
    for (int i = order.size()-1; i > 0; i--) {
        // Choose one agent at random from 0 to i from the list
        int u = rand() % (i+1);

        // Push selected agent to the end to not take it again
        swap(order[i], order[u]);
    }
}


/*----------------------- RANDOM GENERATORS -----------------------*/
/**
 * @brief Based on the number of nodes, the number of edges and the type of random graph
 * to build, the graph generates different random graphs. This is done by writing the
 * parameters into a file and a python3 script reads them to generate the graphs
 * 
 * @param n number of agents
 * @param m number of adges
 * @param k budget
 * @param s seed of the experiments
 * @param p probability (only for Gn,p graphs)
 * @param type 'gnm' | 'erdos' | 'gnp'
 */
void Network::buildRandomGraph(int n, int m, int k, int s, double p, const string& type) {
    fstream file;
    cout << "Writing into the file..." << endl;
    file.open("./randomGraphInstance.txt", ios::out | ios::trunc);
    file << n << endl << m << endl << k << endl << s << endl << p << endl << type << endl;
    cout << "Finished writing!" << endl;
    file.close();

    string command = "python3 randomGenerator.py";
    int status = system(command.c_str());
    if (status < 0 or not WIFEXITED(status)) {
        cout << "Something went wrong while executing " << command << endl;
        exit(1);
    }
    file.open("./randomGraphInstance.txt", ios::in);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            file >> G[i][j];
    file.close();
    convertDirectedToUndirected(G, F);
}

/*----------------------- STRATEGY CHANGES -----------------------*/

/**
 * @brief Buys or adss weight to an edge between two agents. 
 * The first agent passed defines the owner ot the edge
 * 
 * @param u source node
 * @param v destiniy node
 * @param w weight of the edge
 */
void Network::buyEdge(int u, int v, int w) {
    G[u][v] += w;
    F[u][v] += w;
    F[v][u] += w;
    budgets[u] -= w;
    //networkMinCut = minimumGraphCut();
}

/**
 * @brief Removes or takes weight from an edge between two nodes.
 * The first agent passed defines the owner of that edge
 * 
 * @param u 
 * @param v 
 * @param w 
 */
void Network::sellEdge(int u, int v, int w) {
    G[u][v] -= w;
    F[u][v] -= w;
    F[v][u] -= w;
    budgets[u] += w;
    //networkMinCut = minimumGraphCut();
}

/**
 * @brief Changes the strategy of an agent by the vector passed.
 * 
 * @param u agent
 * @param st new strategy
 * @return true the agent strategy changed
 * @return false the agent's strategy hasn't changed
 */
bool Network::setAgentStrategy(int u, const vector<int>& st) {
    bool someChanged = false;
    for (int v = 0; v < n; ++v) {
        if (G[u][v] != st[v]) someChanged = true;
        F[u][v] -= G[u][v];
        F[v][u] -= G[u][v];
        G[u][v] = st[v];
        F[u][v] += st[v];
        F[v][u] += st[v];
    }
    return someChanged;
}

/**
 * @brief Adds new agents to the implicit graph, at the end of it.
 * 
 * @param na number of agents to be added
 */
void Network::addNewAgents(int na) {
    for (int i = 0; i < na; ++i) {
        vector<int> newRow(n, 0);
        G.push_back(newRow);
        F.push_back(newRow);
        n++;
        for (int u = 0; u < n; ++u) {
            G[u].push_back(0);
            F[u].push_back(0);
        }
    }
}

/**
 * @brief Remove agents from the graph. It must be noted that they are removed
 * from the ending of the graph, to make it easier to implement.
 * 
 * @param na number of agents to be removed.
 */
void Network::removeAgents(int na) {
    for (int i = 0; i < na; ++i) {
        G.pop_back();
        F.pop_back();
        n++;
        for (int u = 0; u < n; ++u) {
            G[u].pop_back();
            F[u].pop_back();
        }
    }
}

/*----------------------- NETWORK COMPUTATIONS -----------------------*/

/**
 * @brief Returns the value of the global minimum cut of the passed graph
 * 
 * @param F undirected graph
 * @return double value of the minimum cut
 */
int Network::minimumGraphCut(Graph& F) {
    int n = F.size();
    vector<vector<int>> mat(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            mat[i][j] = F[i][j];
    }
    int minCut = INT_MAX;
    vector<vector<int>> co(n);
    for (int i = 0; i < n; i++) co[i] = {i};
    for (int ph = 1; ph < n; ph++) {
        vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++){
            w[t] = INT_MIN;
            s = t;
            t = max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++) w[i] += mat[t][i];
        }
        minCut = min(minCut, w[t] - mat[t][t]);
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
        mat[0][t] = INT_MIN;
    }
    return minCut;
    /*
    double minCut = 0.0;
    double minUVCut = 0.0;
    for (int u = 0; u < n; ++u) {
        for (int v = u+1; v < n; ++v) {
            minUVCut = minimumSTCut(u, v);
            minCut = max(minCut, minUVCut);
        }
    }
    return minCut;
    */
}

/**
 * @brief Return the degree of an agent
 * 
 * @param F undirected graph
 * @param u agent
 * @return int degree of the agent in the graph
 */
int Network::agentDegree(Graph& F, int u) {
    int degree = 0;
    for (int v = 0; v < n; ++v)
        if (v != u) {
            degree += F[u][v];
        }
    return degree;
}

/**
 * @brief Return the degree of an agent
 * 
 * @param g '0' for directed graph | '1' for undirected graph
 * @param u agent
 * @return int degree of the agent in the graph
 */
int Network::agentDegree(int g, int u) {
    int degree = 0;
    for (int v = 0; v < n; ++v)
        if (v != u)
            degree += g == 0 ? G[u][v] : F[u][v];
    return degree;
}

/**
 * @brief Converts an directed graph into an undirected one where the 
 * weight of multiple edges corresponde to the sum of both
 * 
 * @param G input directed graph
 * @param F output undirected graph
 */
void Network::convertDirectedToUndirected(Graph& G, Graph& F) {
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            int val = G[v][u] + G[u][v];
            F[v][u] = val;
        }
    }
}

/**
 * @brief Computes a maximal j-cluster of the implicit graph. The algorithm works on any case
 * but there might be graphs that don't have a j-cluster. We know though, that every NE has 
 * a (k+1)-cluster, so any NE graph with j = k+1 will always have at least one (k+1)-cluster
 * 
 * @param j minimum edge connectivity of the cluster
 * @return vector<int> list of nodes that belong to a j-cluster
 */
vector<int> Network::computeMaximalCluster(int j) {
    vector<int> maximalCluster;
    for (int i = 0; i < n; ++i) maximalCluster.push_back(i);
    mergeSortByDegree(maximalCluster, 0, n-1);
    int removedAgent = -1;
    bool found = false;
    while (not found) {
        Graph H = Graph(maximalCluster.size(), vector<int>(maximalCluster.size(), 0));
        inducedSubgraph(F, H, maximalCluster, 1);
        int minCut = minimumGraphCut(H);
        if (minCut >= j) {
            removedAgent = maximalCluster[0];
            maximalCluster.erase(maximalCluster.begin());
        }
        else if (maximalCluster.size() < 2) {
            found = true;
        }
        else {
            if (removedAgent != -1) maximalCluster.insert(maximalCluster.begin(), removedAgent);
            found = true;
        }
    }
    return maximalCluster;
}

/**
 * @brief Computes the induced graph by a set of nodes of a given graph
 * 
 * @param F input graph
 * @param H induced graph
 * @param inducingNodes nodes that induce the graph
 * @param g 0 == directed | 1 == undirected
 */
void Network::inducedSubgraph(const Graph& F, Graph& H, vector<int>& inducingNodes, int g) {
    int numberOfNodes = inducingNodes.size();
    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            int capacity = F[inducingNodes[i]][inducingNodes[j]];
            if (capacity > 0) {
                H[i][j] = capacity;
                if (g == 1) H[j][i] = capacity;
            }
        }
    }
}

/*----------------------- EXHAUSTIVE SEARCH -----------------------*/

/**
 * @brief Returns the best response of an agent
 * 
 * @param u agent
 * @param model 'min' | 'avg'
 * @return vector<int> best strategy for agent u
 */
vector<int> Network::agentBestResponse(int u, const string& model) {
    vector<int> bestStrategy(n);
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) Gaux[w][v] = G[w][v];
    
    if (model == "min") {
        pair<int, int> maxUtility = make_pair(0, 0);
        bestResponseMinFlow(Gaux, u, 0, 0, maxUtility, bestStrategy);
    }
    else {
        double maxUtility = 0.0;
        bestResponseAvgFlow(Gaux, u, 0, 0, maxUtility, bestStrategy);
    }
    return bestStrategy;
}

/**
 * @brief Method to quickly compute and apply the best response of an agent
 * 
 * @param u agent
 * @param model 'min' | 'avg'
 */
void Network::computeAndApplyAgentBestResponse(int u, const string& model) {
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    drawGraph(0, "originalGraph");
    vector<int> agentBestStrategy = agentBestResponse(u, model);
    setAgentStrategy(u, agentBestStrategy);
    string agent = to_string(u);
    string title = model + "bestResponseAgent-" + agent;
    printModelsUtility(model);
    printAdjacencyMatrix(0);
    drawGraph(0, title);
}

/*
// NO SE SI ES BORRARA AIXO
vector<int> Network::agentBestResponseMinFlowDeterministic(int u) {
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) Gaux[w][v] = G[w][v];

    vector<int> bestStrategy(n, 0);
    for (int i = 0; i < k; ++i) {
        pair<int, int> maxUtility = make_pair(0, 0);
        int agentToConnectTo = 0;
        bool firstTime = true;
        bool strategyImproved = false;
        int minDegreeAgent = 0;
        int minDegree = INF;
        for (int v = 0; v < n; ++v) {
            if (v != u) {
                Gaux[u][v] += 1;
                Graph Faux(n, vector<int>(n, 0));
                convertDirectedToUndirected(Gaux, Faux);
                pair<int, int> actualUtility = minFlowAgentUtility(Faux, u);
                if (actualUtility.first > maxUtility.first) {
                    maxUtility.first = actualUtility.first;
                    maxUtility.second  = actualUtility.second;
                    agentToConnectTo = v;
                    if (not firstTime)
                        strategyImproved = true;
                    firstTime = false;
                }
                else if (actualUtility.first == maxUtility.first) {
                    if (actualUtility.second > maxUtility.second) {
                        maxUtility.second = actualUtility.second;
                        agentToConnectTo = v;
                        if (not firstTime)
                            strategyImproved = true;
                        firstTime = false;
                    }
                }
                Gaux[u][v] -= 1;
                int deg = agentDegree(Faux, v)-1;
                if (deg < minDegree) {
                    deg = minDegree;
                    minDegreeAgent = v;
                }
            }
        }
        if (not strategyImproved)
            bestStrategy[minDegreeAgent] += 1;
        else
            bestStrategy[agentToConnectTo] += 1;
    }
    return bestStrategy;
}
*/

/**
 * @brief Checks if the agent passed is happy by computing it's best response
 * and checking if it's better or not than it's actual strategy
 * 
 * @param u agent
 * @param agentBestStrategy outpu best strategy for agent u
 * @param model 'min' | 'avg'
 * @return true if agent is happy
 * @return false if agent can still improve
 */
bool Network::isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model) {
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) 
                Gaux[w][v] = G[w][v];

    if (model == "min") {
        auto actualUtility = minFlowAgentUtility(F, u);
        pair<int, int> maxUtility = make_pair(0, 0);
        bestResponseMinFlow(Gaux, u, 0, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        bool isHappy = false;                                       // the agent will be unhappy if the maximum minCut is bigger than the actual
        if (maxUtility.first == actualUtility.first)                // agent will only be happy when he cannot improve the minimum cut
            isHappy = maxUtility.second == actualUtility.second;    // nor the minimum cut of their neighbors
        cout << u << ": (" << actualUtility.first << ", " << actualUtility.second << ")" << " (" << maxUtility.first << ", " << maxUtility.second << ") " << isHappy << endl;
        return isHappy;
    }
    else {
        auto actualUtility = avgFlowAgentUtility(F, u);
        double maxUtility = 0.0;
        bestResponseAvgFlow(Gaux, u, 0, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        bool isHappy = (maxUtility <= actualUtility);
        cout << u << ": " << actualUtility << " " << maxUtility << " " << isHappy << endl;
        return isHappy;
    }
}

/*----------------------- GAME DYNAMICS -----------------------*/

/**
 * @brief Simulates a deterministic Game Dynamics where the agents play
 * in an ascending order: [0, 1, 2, ..., n-1, n]. The game keeps playing
 * until all agents are happy
 * 
 * @param model 'min' |'avg'
 */
void Network::simulateGameDynamics(const string& model) {
    cout << "Original Graph" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);

    bool someoneIsUnhappy = true;
    int rounds = 0;
    while (someoneIsUnhappy) {
        rounds++;
        cout << endl << "Round " << rounds << endl;
        someoneIsUnhappy = false;
        for (int u = 0; u < n; u++) {
            vector<int> agentBestStrategy(n);
            bool isHappy = isAgentHappy(u, agentBestStrategy, model);
            if (not isHappy) {
                someoneIsUnhappy = true;
                setAgentStrategy(u, agentBestStrategy);
            }
        }
    }
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    cout << "Number of rounds played: " << rounds << endl;
}

/**
 * @brief Simulates a deterministic Game Dynamics where the agents play
 * in the order passed as input. The game keeps playing until all agents are happy
 * 
 * @param model 'min' | 'avg'
 * @param agentOrder vector of agent order
 */
void Network::simulateGameDynamics(const string& model, const vector<int>& agentOrder) {
    cout << "Original Graph" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);

    bool someoneIsUnhappy = true;
    int rounds = 0;
    while (someoneIsUnhappy) {
        rounds++;
        cout << endl << "Round " << rounds << endl;
        someoneIsUnhappy = false;
        for (int i = 0; i < n; i++) {
            int u = agentOrder[i];
            vector<int> agentBestStrategy(n);
            bool isHappy = isAgentHappy(u, agentBestStrategy, model);
            if (not isHappy) {
                someoneIsUnhappy = true;
                setAgentStrategy(u, agentBestStrategy);
            }
        }
    }
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    cout << "Number of rounds played: " << rounds << endl;
}

/**
 * @brief Simulates a deterministic Game Dynamics where the agents play
 * in a random order choosen by the Fisher-Yates algorithm. 
 * The game keeps playing until all agents are happy and at every round
 * the order is re-calculated by the algorithm
 * 
 * @param model 'min' | 'avg'
 * @param seed  seed for the random order generator
 */
void Network::simulateGameDynamicsRandomOrder(const string& model, int seed) {
    cout << "Original Graph" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);

    // Initialize random seed
    // srand(time(0));      // uncomment this for truly random
    srand(seed);          // uncomment this to be able to repeat the same random experiments

    bool someoneIsUnhappy = true;
    int rounds = 0;
    while (someoneIsUnhappy) {
        rounds++;
        someoneIsUnhappy = false;
        cout << endl << "Round " << rounds << endl;
        // Set vector with the agents
        vector<int> order(n);
        for (int i = 0; i < n; ++i) order[i] = i;
        shuffleArray(order);

        // The order is taken randomly now
        for (int i = 0; i < order.size(); i++) {
            int u = order[i];
            vector<int> agentBestStrategy(n);
            bool isHappy = isAgentHappy(u, agentBestStrategy, model);
            if (not isHappy) {
                someoneIsUnhappy = true;
                setAgentStrategy(u, agentBestStrategy);
            }
        }
    }
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    cout << "Number of rounds played: " << rounds << endl;
}

/*----------------------- AVG-FLOW MODEL -----------------------*/

/**
 * @brief Returns the utility of an agent in the Avg-FlowNCG model.
 * The utility is the average of the minimum cut between u and all other agents
 * 
 * @param F undirected graph
 * @param u agent
 * @return double agent utility of u
 */
double Network::avgFlowAgentUtility(Graph& F, int u) {
    int n = F.size();
    double utility = 0.0;
    vector<pair<int,int>> minCutNodes;
    for (int v = 0; v < n; ++v)
        if (v != u)
            utility += minimumSTCut(F, minCutNodes, v, u);
    utility /= n-1;
    return utility;
}

/**
 * @brief Returns the social utility of a network in the Avg-FlowNCG model
 * The social utility is the average of the utility of all agents
 * 
 * @param F undirected graph
 * @return double social utility of the network
 */
double Network::avgFlowSocialUtility(Graph& F) {
    int n = F.size();
    double socialU = 0.0;
    for (int u = 0; u < n; ++u)
        socialU += avgFlowAgentUtility(F, u);
    socialU /= n;
    return socialU;
}

/*----------------------- MIN-FLOW MODEL -----------------------*/

/**
 * @brief Returns the number of well-connected neighbours of an agent. This
 * are the number of agents that have a minimum s-t cut to u greater than the
 * global min-cut of the network.
 * 
 * @param F undirected graph
 * @param u agent
 * @return int number of well-connected neighbours
 */
int Network::wellConnectedNeighbours(Graph& F, int u, int minCut) {
    int n = F.size();
    int numWellConnected = 0;
    vector<pair<int,int>> minCutNodes;
    for (int v = 0;  v < n; ++v)
        if (v != u) 
            if (minimumSTCut(F, minCutNodes, v, u) > minCut)
                ++numWellConnected;
    return numWellConnected;
}

/**
 * @brief Returns the utility of an agent in the Min-FlowNCG model
 * The utility is a vector <u1, u2> where u1 is the networ minimum cut and
 * u2 the number of well-connected neighbors of u.
 * 
 * @param F undirected graph
 * @param u agent
 * @return pair<int, int> agent utility of u
 */
pair<int, int> Network::minFlowAgentUtility(Graph& F, int u) {
    int v1 = minimumGraphCut(F);
    int v2 = wellConnectedNeighbours(F, u, v1);
    pair<int, int> utility = make_pair(v1, v2);
    return utility;
}

/**
 * @brief Returns the social utility of a network in the Min-FlowNCG model
 * The utility is the minimum cut of the network.
 * 
 * @param F undirected graph
 * @return double social utility of the network
 */
int Network::minFlowSocialUtility(Graph& F) {
    return minimumGraphCut(F);
}

/*----------------------- INPUT/OUTPUT -----------------------*/

/**
 * @brief Prints through the terminal by cout the adjacency matrix
 * of the attributes in the class
 * 
 * @param g '0' for directed graph | '1' for undirected graph
 */
void Network::printAdjacencyMatrix(int g) {
    cout << "   ";
    for (int v = 0; v < n; ++v)
        cout << " " << v;
    cout << endl << "    ";
    for (int v = 0; v < n; ++v)
        cout << "-" << " ";
    cout << endl;
    for (int v = 0; v < n; ++v) {
        cout << v << " | ";
        for (int u = 0; u < n; ++u) {
            if (g == 0)
                cout << G[u][v] << " ";
            else
                cout << F[u][v] << " ";
        }
        cout << endl;
    }
}

/**
 * @brief Prints through the terminal by cout the adjacency matrix
 * of the attributes in the class
 * 
 * @param G graph to print it's adcjacency
 */
void Network::printAdjacencyMatrix(const Graph& G) {
    int n = G.size();
    for (int v = 0; v < n; ++v)
        cout << " " << v;
    cout << endl << "    ";
    for (int v = 0; v < n; ++v)
        cout << "-" << " ";
    cout << endl;
    for (int v = 0; v < n; ++v) {
        cout << v << " | ";
        for (int u = 0; u < n; ++u) {
                cout << G[u][v] << " ";
        }
        cout << endl;
    }
}

/**
 * @brief Prints through the terminal by cout the social and
 * agent utility of the Min-FlowNCG and Avg-FLowNCG models
 * 
 * @param model 'min' | 'avg'
 */
void Network::printModelsUtility(const string& model) {
    if (model == "min") {
        cout << "minFlow-NCG Model" << endl;
        int minCut = 0.0;
        for (int u = 0; u < n; ++u) {
            auto agentUtility = minFlowAgentUtility(F, u);
            minCut = agentUtility.first;
            cout << " - Agent " << u << ": (" << agentUtility.first << ", " << agentUtility.second << ")" << endl;
        }
        cout << " - Social Utility: " << minCut << endl; 
    }
    else if (model == "avg") {
        cout << "avgFlow-NCG Model" << endl;
        for (int u = 0; u < n; ++u)
            cout << " - Agent " << u << ": " << avgFlowAgentUtility(F, u) << endl;
        cout << " - Social Utility: " << avgFlowSocialUtility(F) << endl;
        cout << "------------------------" << endl;
    }
    else {
        cout << "avgFlow-NCG Model" << endl;
        for (int u = 0; u < n; ++u)
            cout << " - Agent " << u << ": " << avgFlowAgentUtility(F, u) << endl;
        cout << " - Social Utility: " << avgFlowSocialUtility(F) << endl;
        cout << "------------------------" << endl;

        cout << "minFlow-NCG Model" << endl;
        double minCut = 0.0;
        for (int u = 0; u < n; ++u) {
            auto agentUtility = minFlowAgentUtility(F, u);
            minCut = agentUtility.first;
            cout << " - Agent " << u << ": (" << agentUtility.first << ", " << agentUtility.second << ")" << endl;
        }
        cout << " - Social Utility: " << minCut << endl;
    }
}

void Network::printModelsUtility(Graph& F, const string& model) {
    int n = F.size();
    if (model == "min") {
        cout << "minFlow-NCG Model" << endl;
        int minCut = 0.0;
        for (int u = 0; u < n; ++u) {
            auto agentUtility = minFlowAgentUtility(F, u);
            minCut = agentUtility.first;
            cout << " - Agent " << u << ": (" << agentUtility.first << ", " << agentUtility.second << ")" << endl;
        }
        cout << " - Social Utility: " << minCut << endl; 
    }
    else if (model == "avg") {
        cout << "avgFlow-NCG Model" << endl;
        for (int u = 0; u < n; ++u)
            cout << " - Agent " << u << ": " << avgFlowAgentUtility(F, u) << endl;
        cout << " - Social Utility: " << avgFlowSocialUtility(F) << endl;
        cout << "------------------------" << endl;
    }
    else {
        cout << "avgFlow-NCG Model" << endl;
        for (int u = 0; u < n; ++u)
            cout << " - Agent " << u << ": " << avgFlowAgentUtility(F, u) << endl;
        cout << " - Social Utility: " << avgFlowSocialUtility(F) << endl;
        cout << "------------------------" << endl;

        cout << "minFlow-NCG Model" << endl;
        double minCut = 0.0;
        for (int u = 0; u < n; ++u) {
            auto agentUtility = minFlowAgentUtility(F, u);
            minCut = agentUtility.first;
            cout << " - Agent " << u << ": (" << agentUtility.first << ", " << agentUtility.second << ")" << endl;
        }
        cout << " - Social Utility: " << minCut << endl;
    }
}

/**
 * @brief Draws the graph by writing it into a file and calling
 * a python scrip that draws and saves it 
 * 
 * @param g '0' for directed graph | '1' for undirected graph
 * @param filename name of the file to save the graph drawing into
 */
void Network::drawGraph(int g, const string& filename) {
    writeGraphToFile(g, filename);
    cout << "Drawing and saving graph..." << endl;
    string command = "python3 networkDrawer.py " + filename + " " + to_string(g);
    int status = system(command.c_str());
    if (status < 0 or not WIFEXITED(status)) {
        cout << "Something went wrong when executing " << command << endl;
        exit(1);
    }
    else cout << "Completed!" << endl;
}

void Network::readGraphFromFile(const string& filename) {
    ifstream file;
    string path = "./" + filename + ".txt";
    file.open(path);
    for (int u = 0; u < n; ++u) {
        for(int v = 0; v < n; ++v) {
            int w = int(file.get());
            G[u][v] = w;
        }
    }
    convertDirectedToUndirected(G, F);
}

void Network::writeGraphToFile(int g, const string& filename) {
    ofstream file;
    string path = "./" + filename + ".txt";
    cout << "Writing into the file..." << endl;
    file.open(path, ios::out | ios::trunc);
    file << n << endl;
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (g == 0)
                file << G[u][v] << " ";
            else
                file << F[u][v] << " ";
        }
        file << endl;
    }
    file.close();
    cout << "Completed!" << endl;
}

/*----------------------- AUXILIAR -----------------------*/
// NO FUNCIONA
bool Network::isCycleOptimumGraph() {
    bool isCycle = (G[n-1][0] > 0);
    int u = 0;
    while (u < n-1 and isCycle) {
        if (G[u][u+1])
            isCycle = false;
        u++;
    }
    return isCycle;
}

vector<int> Network::numberOfEdges(int g) {
    vector<int> edges(k, 0);
    if (g == 0) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                int capacity = G[u][v];
                if (capacity) ++edges[capacity-1];  // the vector starts on 0 but the weights on 1
            }
        }
    }
    else {
        for (int u = 0; u < n; u++) {
            // In the undirected graph the data is repeated
            for (int v = u+1; v < n; v++) {
                int capacity = F[u][v];
                if (capacity) ++edges[capacity-1];
            }
        }
    }
    return edges;
}
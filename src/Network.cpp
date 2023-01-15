#include "Network.hh"
#include <queue>
#include <climits>
#include <algorithm>

/*----------------------- CONSTRUCTORS -----------------------*/

Network::Network() { 
    G = Graph(0, vector<int>(0, 0));
    networkMinCut = 0;
    F = Graph(0, vector<int>(0, 0));
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

/*----------------------- AUXILIARS -----------------------*/

void Network::eraseAllConnections(Graph& G) {
    int m = G.size();
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            G[i][j] = 0;
}

// PRE: G is an adjacency matrix, s and t are vertices from G and
//      path is variable that will store the path from s and t (if it exists)
// POST: True if there is an s-t path, False otherwise
bool Network::BFS(const Graph& residualG, int s, int t, vector<int>& path, int& minPathCapacity) {
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

void Network::DFS(const Graph& residualG, int s, vector<bool>& isInMinimumCut) {
    isInMinimumCut[s] = true;
    for (int u = 0; u < n; ++u) {
        if (residualG[s][u] and not isInMinimumCut[u])
            DFS(residualG, u, isInMinimumCut);
    }
}

double Network::minimumSTCut(Graph& F, int s, int t) {

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
    DFS(residualG, s, isInMinimumCut);

    double minCut = 0.0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (isInMinimumCut[i] and not isInMinimumCut[j] and F[i][j] > 0)
                minCut += F[i][j];

    return minCut;
    
}

void Network::bestResponseMinFlow(Graph& GR, int u, int kUsed, pair<int, int>& maxUtility, vector<int>& maxStrategy) {
    if (kUsed == k) {
        Graph FR(n, vector<int>(n, 0));
        convertDirectedToUndirected(GR, FR);
        auto actualUtility = minFlowAgentUtility(FR, u);
        if (actualUtility.first > maxUtility.first) {
            maxUtility.first = actualUtility.first;
            maxUtility.second = actualUtility.second;
            for (int v = 0; v < n; ++v)
                maxStrategy[v] = GR[u][v]; 
        }
        else {
            if (actualUtility.second > maxUtility.second) {
                maxUtility.first = actualUtility.first;
                maxUtility.second = actualUtility.second;
                for (int v = 0; v < n; ++v)
                    maxStrategy[v] = GR[u][v];
            }
        }
    }
    else {
        for (int v = 0; v < n; ++v) {
            if (v != u) {
                GR[u][v] += 1;
                kUsed += 1;
                bestResponseMinFlow(GR, u, kUsed, maxUtility, maxStrategy);
                kUsed -= 1;
                GR[u][v] -= 1;
            }
        }
    }
}

void Network::bestResponseAvgFlow(Graph& GR, int u, int kUsed, double& maxUtility, vector<int>& maxStrategy) {
    if (kUsed == k) {
        Graph FR(n, vector<int>(n, 0));
        convertDirectedToUndirected(GR, FR);
        double actualUtility = avgFlowAgentUtility(FR, u);
        if (actualUtility > maxUtility) {
            maxUtility = actualUtility;
            for (int v = 0; v < n; ++v)
                maxStrategy[v] = GR[u][v];
        }
    }
    else {
        for (int v = 0; v < n; ++v) {
            if (v != u) {
                GR[u][v] += 1;
                kUsed += 1;
                bestResponseAvgFlow(GR, u, kUsed, maxUtility, maxStrategy);
                kUsed -= 1;
                GR[u][v] -= 1;
            }
        }
    }
}

void Network::buyEdge(int u, int v, int w) {
    G[u][v] += w;
    F[u][v] += w;
    F[v][u] += w;
    budgets[u] -= w;
    //networkMinCut = minimumGraphCut();
}

void Network::sellEdge(int u, int v, int w) {
    G[u][v] -= w;
    F[u][v] -= w;
    F[v][u] -= w;
    budgets[u] += w;
    //networkMinCut = minimumGraphCut();
}

vector<int> Network::agentBestResponse(int u, const string& model) {
    vector<int> bestStrategy(n);
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) Gaux[w][v] = G[w][v];
    
    if (model == "min") {
        pair<int, int> maxUtility = make_pair(0, 0);
        bestResponseMinFlow(Gaux, u, 0, maxUtility, bestStrategy);
    }
    else {
        double maxUtility = 0.0;
        bestResponseAvgFlow(Gaux, u, 0, maxUtility, bestStrategy);
    }
    return bestStrategy;
}

bool Network::isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model) {
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) 
                Gaux[w][v] = G[w][v];

    if (model == "min") {
        auto actualUtility = minFlowAgentUtility(F, u);
        pair<int, int> maxUtility = make_pair(0, 0);
        bestResponseMinFlow(Gaux, u, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        bool isHappy = false;
        if (maxUtility.first <= actualUtility.first)    // first we want to maximize de the first component of the vector
            isHappy = true;
        else                                            // if the first component is not bigger, then try to maximize the second one
            isHappy = maxUtility.second <= actualUtility.second;
        //cout << "(" << actualUtility.first << ", " << actualUtility.second << ")" << " (" << maxUtility.first << ", " << maxUtility.second << ") " << isHappy << endl;
        return isHappy;
    }
    else {
        auto actualUtility = avgFlowAgentUtility(F, u);
        double maxUtility = 0.0;
        bestResponseAvgFlow(Gaux, u, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        bool isHappy = (maxUtility <= actualUtility);
        //cout << actualUtility << " " << maxUtility << " " << isHappy << endl;
        return isHappy;
    }
}

double Network::minimumGraphCut(Graph& F) {
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

void Network::printAdjacencyMatrix(const Graph& G) {
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

void Network::printModelsUtility(const string& model) {
    if (model == "min") {
        cout << "minFlow-NCG Model" << endl;
        double minCut = 0.0;
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

void Network::simulateGameDynamics(const string& model) {
    bool someoneIsUnhappy = true;
    while (someoneIsUnhappy) {
        someoneIsUnhappy = false;
        for (int u = 0; u < n; u++) {
            vector<int> agentBestStrategy(n);
            bool isHappy = isAgentHappy(u, agentBestStrategy, model);
            if (not isHappy) {
                someoneIsUnhappy = true;
                setAgentStrategy(u, agentBestStrategy);
                /*cout << "Agent " << u << " is not happy ";
                for (int i = 0; i < agentBestStrategy.size(); ++i)
                    cout << agentBestStrategy[i] << " ";
                cout << endl;*/
            }
        }
        //printAdjacencyMatrix(0);
        //cout << endl;
    }
}

void Network::convertDirectedToUndirected(Graph& G, Graph& F) {
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            int val = G[v][u] + G[u][v];
            F[v][u] = val;
        }
    }
}


/*----------------------- AVG-FLOW MODEL -----------------------*/

double Network::avgFlowAgentUtility(Graph& F, int u) {
    double utility = 0.0;
    for (int v = 0; v < n; ++v)
        if (v != u)
            utility += minimumSTCut(F, v, u);
    utility /= n-1;
    return utility;
}

double Network::avgFlowSocialUtility(Graph& F) {
    double socialU = 0.0;
    for (int u = 0; u < n; ++u)
        socialU += avgFlowAgentUtility(F, u);
    socialU /= n;
    return socialU;
}

/*----------------------- MIN-FLOW MODEL -----------------------*/

int Network::wellConnectedNeighbours(Graph& F, int u) {
    int numWellConnected = 0;
    for (int v = 0;  v < n; ++v)
        if (v != u)
            if (minimumSTCut(F, v, u) > minimumGraphCut(F))
                ++numWellConnected;
    return numWellConnected;
}

pair<int, int> Network::minFlowAgentUtility(Graph& F, int u) {
    int v1 = minimumGraphCut(F);
    int v2 = wellConnectedNeighbours(F, u);
    pair<int, int> utility = make_pair(v1, v2);
    return utility;
}

double Network::minFlowSocialUtility(Graph& F) {
    return minimumGraphCut(F);
}
#include "GeneralBudgetFlowGame.hh"

/*----------------------- CONSTRUCTORS -----------------------*/

GeneralBudgetFlowGame::GeneralBudgetFlowGame() {
    n = 0;
    G = Graph(0, vector<int>(0, 0));
    F = Graph(0, vector<int>(0, 0));
}

GeneralBudgetFlowGame::GeneralBudgetFlowGame(int nAgents, vector<int>& budget) {
    n = nAgents;
    k = budget;
    G = Graph(0, vector<int>(0, 0));
    F = Graph(0, vector<int>(0, 0));
}

GeneralBudgetFlowGame::~GeneralBudgetFlowGame() { }

/*----------------------- BEST RESPONSE MODELS -----------------------*/

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
void GeneralBudgetFlowGame::bestResponseMinFlow(Graph& GR, int u, vector<int>& budgetLeft, int lastVisited, pair<int, int>& maxUtility, vector<int>& maxStrategy) {
    if (budgetLeft[u] == 0) {
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
                budgetLeft[u] -= 1;
                bestResponseMinFlow(GR, u, budgetLeft, v, maxUtility, maxStrategy);
                budgetLeft[u] += 1;
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
void GeneralBudgetFlowGame::bestResponseAvgFlow(Graph& GR, int u, vector<int>& budgetLeft, int lastVisited, double& maxUtility, vector<int>& maxStrategy) {
    if (budgetLeft[u] == 0) {
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
                budgetLeft[u] -= 1;
                bestResponseAvgFlow(GR, u, budgetLeft, v, maxUtility, maxStrategy);
                budgetLeft[u] += 1;
                GR[u][v] -= 1;
            }
        }
    }
}

/*----------------------- NETWORK COMPUTATIONS -----------------------*/

/**
 * @brief Computes a maximal j-cluster of the implicit graph. The algorithm works on any case
 * but there might be graphs that don't have a j-cluster. We know though, that every NE has 
 * a (k+1)-cluster, so any NE graph with j = k+1 will always have at least one (k+1)-cluster
 * 
 * @param j minimum edge connectivity of the cluster
 * @return vector<int> list of nodes that belong to a j-cluster
 */
vector<int> GeneralBudgetFlowGame::computeMaximalCluster(int j) {
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

bool GeneralBudgetFlowGame::isNetworkMaximalCluster(int j) {
    bool isMaximal = true;
    for (int i = 0; i < n and isMaximal; ++i) {
        vector<int> remainingNodes(n-1);
        for (int j = 0; j < n-1; ++j)
            if (j >= i) remainingNodes[j] = j+1;
            else remainingNodes[j] = j;
        Graph H = Graph(n-1, vector<int>(n-1, 0));
        inducedSubgraph(F, H, remainingNodes, 1);
        int minCut = minimumGraphCut(H);
        if (minCut >= j) 
            isMaximal = false;
    }
    return isMaximal;
}

/*----------------------- BEST RESPONSE -----------------------*/

/**
 * @brief Returns the best response of an agent
 * 
 * @param u agent
 * @param model 'min' | 'avg'
 * @return vector<int> best strategy for agent u
 */
vector<int> GeneralBudgetFlowGame::agentBestResponse(int u, const string& model) {
    vector<int> bestStrategy(n);
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u) Gaux[w][v] = G[w][v];
    
    if (model == "min") {
        pair<int, int> maxUtility = make_pair(0, 0);
        vector<int> budgetLeft(n);
        copy(k.begin(), k.end(), budgetLeft.begin());
        bestResponseMinFlow(Gaux, u, budgetLeft, 0, maxUtility, bestStrategy);
    }
    else {
        double maxUtility = 0.0;
        vector<int> budgetLeft(n);
        copy(k.begin(), k.end(), budgetLeft.begin());
        bestResponseAvgFlow(Gaux, u, budgetLeft, 0, maxUtility, bestStrategy);
    }
    return bestStrategy;
}

/**
 * @brief Method to quickly compute and apply the best response of an agent
 * 
 * @param u agent
 * @param model 'min' | 'avg'
 */
void GeneralBudgetFlowGame::computeAndApplyAgentBestResponse(int u, const string& model) {
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
bool GeneralBudgetFlowGame::isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model) {
    Graph Gaux(n, vector<int>(n, 0));
    for (int w = 0; w < n; ++w)
        for (int v = 0; v < n; ++v)
            if (w != u)
                Gaux[w][v] = G[w][v];

    bool isHappy = false;
    if (model == "min") {
        auto actualUtility = minFlowAgentUtility(F, u);
        pair<int, int> maxUtility = make_pair(0, 0);
        vector<int> budgetLeft(n);
        copy(k.begin(), k.end(), budgetLeft.begin());
        bestResponseMinFlow(Gaux, u, budgetLeft, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        // the agent will be unhappy if the maximum minCut is bigger than the actual
        if (maxUtility.first == actualUtility.first)                // agent will only be happy when he cannot improve the minimum cut
            isHappy = (maxUtility.second == actualUtility.second);    // nor the minimum cut of their neighbors
        cout << u << ": (" << actualUtility.first << ", " << actualUtility.second << ")" << " (" << maxUtility.first << ", " << maxUtility.second << ") " << isHappy << endl;
    }
    else {
        auto actualUtility = avgFlowAgentUtility(F, u);
        double maxUtility = 0.0;
        vector<int> budgetLeft(n);
        copy(k.begin(), k.end(), budgetLeft.begin());
        bestResponseAvgFlow(Gaux, u, budgetLeft, 0, maxUtility, agentBestStrategy);
        // If agent is happy there is no better move than the actual
        // so then the maximum utility is less or equal than the actual one
        isHappy = (maxUtility <= actualUtility);
        cout << u << ": " << actualUtility << " " << maxUtility << " " << isHappy << endl;
    }
    return isHappy;
}

/*----------------------- GAME DYNAMICS -----------------------*/

/**
 * @brief Simulates a deterministic Game Dynamics where the agents play
 * in an ascending order: [0, 1, 2, ..., n-1, n]. The game keeps playing
 * until all agents are happy
 * 
 * @param model 'min' |'avg'
 * @return number of rounds played
 */
int GeneralBudgetFlowGame::simulateGameDynamics(const string& model) {
    /*
    cout << "Original Graph" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    */
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
    /*
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    cout << "Number of rounds played: " << rounds << endl;
    */
    return rounds;
}

/**
 * @brief Simulates a deterministic Game Dynamics where the agents play
 * in the order passed as input. The game keeps playing until all agents are happy
 * 
 * @param model 'min' | 'avg'
 * @param agentOrder vector of agent order
 */
int GeneralBudgetFlowGame::simulateGameDynamics(const string& model, const vector<int>& agentOrder) {
    /*
    cout << "Original Graph" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    */
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
    /*
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Graph changed by agents best response" << endl;
    printAdjacencyMatrix(0);
    printModelsUtility(model);
    cout << "Number of rounds played: " << rounds << endl;
    */
    return rounds;
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
int GeneralBudgetFlowGame::simulateGameDynamicsRandomOrder(const string& model, int seed) {
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
    return rounds;
}
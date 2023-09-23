#ifndef BASEFLOWGAME_HH
#define BASEFLOWGAME_HH

#include "Network.hh"

class BaseFlowGame : public Network {

    private:
        int k = 0;

        // Best Response Models
        void bestResponseMinFlow(Graph& GR, int u, int budgetUsed, int lastVisited, pair<int, int>& maxUtility, vector<int>& maxStrategy);
        void bestResponseAvgFlow(Graph& GR, int u, int budgetUsed, int lastVisited, double& maxUtility, vector<int>& maxStrategy);

    public:
        // Constructors
        BaseFlowGame();
        BaseFlowGame(int nAgents, int budget);
        ~BaseFlowGame();

        // Network Computations
        vector<int> computeMaximalCluster(int j);
        bool isNetworkMaximalCluster(int j);

        // Best Response
        vector<int> agentBestResponse(int u, const string& model);
        void computeAndApplyAgentBestResponse(int u, const string& model);
        bool isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model);
        //vector<int> agentBestResponseMinFlowDeterministic(int u);
        //vector<int> agentBestResponseAvgFlowDeterministic(int u);
        
        // Game Dynamics
        int simulateGameDynamics(const string& model);
        int simulateGameDynamics(const string& model, const vector<int>& agentOrder);
        int simulateGameDynamicsRandomOrder(const string& model, int seed);

};

#endif
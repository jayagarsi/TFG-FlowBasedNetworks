#ifndef GENERALBUDGETFLOWGAME_HH
#define GENERALBUDGETFLOWGAME_HH

#include "Network.hh"

class GeneralBudgetFlowGame : public Network {

    private:
        vector<int> k;

        // Best Response Models
        void bestResponseMinFlow(Graph& GR, int u, vector<int>& budgetLeft, int lastVisited, pair<int, int>& maxUtility, vector<int>& maxStrategy);
        void bestResponseAvgFlow(Graph& GR, int u, vector<int>& budgetLeft, int lastVisited, double& maxUtility, vector<int>& maxStrategy);
    
    public:
        // Constructors
        GeneralBudgetFlowGame();
        GeneralBudgetFlowGame(int nAgents, vector<int>& budget);
        ~GeneralBudgetFlowGame();

        // Network Computations
        vector<int> computeMaximalCluster(int j);
        bool isNetworkMaximalCluster(int j);

        // Best Response
        vector<int> agentBestResponse(int u, const string& model);
        void computeAndApplyAgentBestResponse(int u, const string& model);
        bool isAgentHappy(int u, vector<int>& agentBestStrategy, const string& model);

        // Game Dynamics
        int simulateGameDynamics(const string& model);
        int simulateGameDynamics(const string& model, const vector<int>& agentOrder);
        int simulateGameDynamicsRandomOrder(const string& model, int seed);

};

#endif
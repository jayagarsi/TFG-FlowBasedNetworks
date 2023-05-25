#include "Network.hh"
#include <fstream>
#include <string>
using namespace std;

int sumValues(const vector<int> &array) {
    int sum = 0;
    for (int i = 0; i < array.size(); ++i)
        sum += array[i];
    return sum;
}

int main() {
    int n, m, k, s, agentsToChange;
    int direction = 0;
    double p;
    string model, graphType, order, modification, csvFilename, experiment;
    string name = "originalGraph";
    ifstream params;
    params.open("./dynamicsParameters.txt", ios::in);
    params >> n >> k >> m >> p >> s >> model >> graphType >> order >> modification >> csvFilename >> experiment;
    if (modification == "exp" or modification == "red") params >> agentsToChange;
    Network G(n, k);
    if (graphType == "gnm" or graphType == "gnp") 
        G.buildRandomGraph(n, m, k, s, p, graphType);
    if (experiment == "fixedNincreasingK") name = name + "-k" + to_string(k);
    else if (experiment == "fixedKincreasingN") name = name + "-n" + to_string(n);
    G.drawGraph(direction, name);

    double originalUtility, equilibriumUtility;
    if (model == "min")
        originalUtility = G.minFlowSocialUtility();
    else if (model == "avg")
        originalUtility = G.avgFlowSocialUtility();

    int numberOfRounds = 0;
    if (order == "ra") numberOfRounds = G.simulateGameDynamicsRandomOrder(model, s);
    else if (order == "rr") numberOfRounds = G.simulateGameDynamics(model);
    else if (order == "pr") {
        vector<int> agentOrder(n);
        for (int i = 0; i < n; ++i)
            agentOrder[i] = n-i-1;
        numberOfRounds = G.simulateGameDynamics(model, agentOrder);
    }
    if (model == "min")
        equilibriumUtility = G.minFlowSocialUtility();
    else if (model == "avg")
        equilibriumUtility = G.avgFlowSocialUtility();

    vector<int> edgesDirected = G.numberOfEdges(0);
    vector<int> edgesUndirected = G.numberOfEdges(1);

    int numEdgesDirected = sumValues(edgesDirected);
    int numEdgesUndirected = sumValues(edgesUndirected);

    vector<int> maximalCluster = G.computeMaximalCluster(k + 1);
    int maximalClusterSize = maximalCluster.size();

    if (experiment == "fixedNincreasingK") name = model + "Flow-" + order + "-k" + to_string(k);
    else if (experiment == "fixedKincreasingN") name = model + "Flow-" + order + "-n" + to_string(n);
    else if (experiment == "networkExpansion" or experiment == "networkReduction") { 
        name = "originalDynamics-" + order + "-" + model + "Flow";
        G.drawGraph(direction, name);
        int newN = n;
        if (experiment == "networkExpansion") {
            newN += agentsToChange;
            G.addNewAgents(agentsToChange);
            name = "expandedDynamics-";
        }
        else if (experiment == "networkReduction") {
            newN -= agentsToChange;
            G.removeAgents(agentsToChange);
            name = "reducedDynamics-";
        }

        double changedOriginalUtility, changedEquilibriumUtility;
        if (model == "min")
            changedOriginalUtility = G.minFlowSocialUtility();
        else if (model == "avg")
            changedOriginalUtility = G.avgFlowSocialUtility();

        int changedNumberOfRounds = 0;
        if (order == "ra") changedNumberOfRounds = G.simulateGameDynamicsRandomOrder(model, s);
        else if (order == "rr") changedNumberOfRounds = G.simulateGameDynamics(model);
        else if (order == "pr") {
            vector<int> agentOrder(n);
            for (int i = 0; i < n; ++i)
                agentOrder[i] = n-i-1;
            changedNumberOfRounds = G.simulateGameDynamics(model, agentOrder);
        }
        if (model == "min")
            changedEquilibriumUtility = G.minFlowSocialUtility();
        else if (model == "avg")
            changedEquilibriumUtility = G.avgFlowSocialUtility();

        vector<int> changedEdgesDirected = G.numberOfEdges(0);
        vector<int> changedEdgesUndirected = G.numberOfEdges(1);

        int changedNumEdgesDirected = sumValues(changedEdgesDirected);
        int changedNumEdgesUndirected = sumValues(changedEdgesUndirected);

        vector<int> changedMaximalCluster = G.computeMaximalCluster(k + 1);
        int changedMaximalClusterSize = changedMaximalCluster.size();

        name += order + "-" + model + "Flow-n" + to_string(agentsToChange);
        G.drawGraph(direction, name);

        ofstream file;
        string path = "./" + csvFilename;
        file.open(path, ios::out | ios::app);
        file << originalUtility << "," << equilibriumUtility << "," << numberOfRounds << "," << numEdgesDirected << "," << numEdgesUndirected << "," << maximalClusterSize;
        file << changedOriginalUtility << "," << changedEquilibriumUtility << "," << changedNumberOfRounds << "," << changedNumEdgesDirected << "," << changedNumEdgesUndirected << "," << changedMaximalClusterSize << endl;
        file.close();
    }

    // Append results to file
    if (experiment == "fixedNincreasingK" or experiment == "fixedKincreasingN") {
        ofstream file;
        string path = "./" + csvFilename;
        file.open(path, ios::out | ios::app);
        file << originalUtility << "," << equilibriumUtility << "," << numberOfRounds << "," << numEdgesDirected << "," << numEdgesUndirected << "," << maximalClusterSize << endl;
        file.close();
    }
}
import networkx as nx
import random as rd

if __name__ == '__main__':
    
    ### Read parameters of the graph
    file = open("randomGraphInstance.txt", "r")
    n = int(file.readline())
    m = int(file.readline())
    k = int(file.readline())
    s = int(file.readline())
    p = float(file.readline())
    type = file.readline()[:-1]
    file.close()

    G = nx.DiGraph()
    ### Generate different types of graph based on the input
    if type == "gnm":
        G = nx.gnm_random_graph(n, m, s, True)
    elif type == "gnp":
        G = nx.erdos_renyi_graph(n, p, s, True)

    ### Generate the adjacency matrix
    adjacencyMatrix = [[0 for i in range(n)] for j in range(n)]
    for u, v, in G.edges():
        adjacencyMatrix[u][v] = 1

    ### Assign to each existing edge a random value between 1 and available budget
    rd.seed(s)      # this allows the experiments to be repeated for same seeds
    for i in range(n):
        availableBudget = k
        for j in range(n):
            if (adjacencyMatrix[i][j] > 0):
                if (availableBudget > 1):
                    selectedWeight = rd.randint(1, availableBudget)
                    adjacencyMatrix[i][j] = selectedWeight
                    availableBudget -= selectedWeight
                elif (availableBudget == 1):
                    adjacencyMatrix[i][j] = 1
                    availableBudget = 0
                else:
                    adjacencyMatrix[i][j] = 0
    
    file = open("randomGraphInstance.txt", "w")
    for row in adjacencyMatrix:
        file.write(' '.join(str(elem) for elem in row) + '\n')

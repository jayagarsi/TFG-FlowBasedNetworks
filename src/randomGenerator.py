import networkx as nx
import my_draw_networx_edge_labels as my_nx
import matplotlib.pyplot as plt

if __name__ == '__main__':
    file = open("graphInstance.txt", "r")
    n = int(file.readline())
    adjacencyMatrix = []
    for i in range(n):
        row = list(file.readline())
        row = [int(w) for w in row[:-1]]
        #print(row)
        adjacencyMatrix.append(row)
    file.close()

    #print(adjacencyMatrix)

    G = nx.DiGraph()
    for i in range(n):
        for j in range(n):
            edgeWeight = adjacencyMatrix[i][j]
            if (edgeWeight != 0):
                G.add_edge(i, j, weight = edgeWeight)

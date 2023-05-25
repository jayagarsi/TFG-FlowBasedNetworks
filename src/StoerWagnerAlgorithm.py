import networkx as nx
import my_draw_networx_edge_labels as my_nx
import matplotlib.pyplot as plt
import sys

if __name__ == '__main__':
    filename = sys.argv[1]
    path = "./" + filename + ".txt"
    file = open(path, "r")
    n = int(file.readline())
    adjacencyMatrix = []
    for i in range(n):
        row = file.readline()
        row = [int(w) for w in row[:-1].split()]
        # print(row)
        adjacencyMatrix.append(row)
    file.close()
    G = nx.Graph()
    for i in range(n):
        for j in range(n):
            edgeWeight = adjacencyMatrix[i][j]
            if (edgeWeight != 0):
                G.add_edge(i, j, weight = edgeWeight)
    minCut, partition = nx.stoer_wagner(G)
    file = open(path, "w")
    file.write(str(minCut))
    file.close()

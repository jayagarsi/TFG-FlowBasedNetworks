import networkx as nx
import my_draw_networx_edge_labels as my_nx
import matplotlib.pyplot as plt

def drawDirectedNetwork(G):
    #https://stackoverflow.com/questions/22785849/drawing-multiple-edges-between-two-nodes-with-networkx
    pos=nx.spring_layout(G,seed=5)
    fig, ax = plt.subplots()

    # Print nodes amb la seva etiqueta
    nx.draw_networkx_nodes(G, pos, ax=ax)
    nx.draw_networkx_labels(G, pos, ax=ax)

    # Print arestes d'anada i de tornada
    arc_rad = 0.25
    curved_edges = [edge for edge in G.edges() if reversed(edge) in G.edges()]
    straight_edges = list(set(G.edges()) - set(curved_edges))
    nx.draw_networkx_edges(G, pos, ax=ax, edgelist=straight_edges)
    nx.draw_networkx_edges(G, pos, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad = {arc_rad}')

    # Print arestes amb la seva etiqueta
    edge_weights = nx.get_edge_attributes(G,'weight')
    curved_edge_labels = {edge: edge_weights[edge] for edge in curved_edges}
    straight_edge_labels = {edge: edge_weights[edge] for edge in straight_edges}
    my_nx.my_draw_networkx_edge_labels(G, pos, ax=ax, edge_labels=curved_edge_labels,rotate=False,rad = arc_rad)
    nx.draw_networkx_edge_labels(G, pos, ax=ax, edge_labels=straight_edge_labels,rotate=False)
    plt.savefig("graph.png")

if __name__ == '__main__':
    file = open("graphInstance.txt", "r")
    n = int(file.readline())
    adjacencyMatrix = []
    for i in range(n):
        row = list(file.readline())
        row = [int(w) for w in row[:-1]]
        adjacencyMatrix.append(row)

    file.close()

    G = nx.DiGraph()
    for i in range(n):
        for j in range(n):
            edgeWeight = adjacencyMatrix[i][j]
            if (edgeWeight != 0):
                G.add_edge(i, j, weight = edgeWeight)
    drawDirectedNetwork(G)

import networkx as nx
import my_draw_networx_edge_labels as my_nx
import matplotlib.pyplot as plt
import sys

def drawDirectedNetwork(G, filename):
    #https://stackoverflow.com/questions/22785849/drawing-multiple-edges-between-two-nodes-with-networkx
    pos=nx.spring_layout(G,seed=5)
    fig, ax = plt.subplots()

    # Print arestes amb la seva etiqueta
    nx.draw_networkx_nodes(G, pos, ax=ax)
    nx.draw_networkx_labels(G, pos, ax=ax)
    nx.draw_networkx_edges(G, pos, ax=ax, edgelist=G.edges())

    edge_weights = nx.get_edge_attributes(G,'weight')
    nx.draw_networkx_edge_labels(G, pos, ax=ax, edge_labels=edge_weights,rotate=False)
    plt.savefig(filename + ".png")

if __name__ == '__main__':
    G = nx.Graph()
    G.add_nodes_from(range(0, 6))
    G.add_edge(0, 1, weight=10)
    G.add_edge(0, 5, weight=8)
    G.add_edge(1, 2, weight=3)
    G.add_edge(1, 4, weight=2)
    G.add_edge(1, 5, weight=3)
    G.add_edge(2, 3, weight=5)
    G.add_edge(2, 4, weight=4)
    G.add_edge(2, 5, weight=2)
    G.add_edge(3, 4, weight=7)
    G.add_edge(4, 5, weight=3)
    drawDirectedNetwork(G, "gomoryhu-original")
    
    F = nx.gomory_hu_tree(G, capacity="weight")
    drawDirectedNetwork(F, "gomoryhu-1")

    # G.add_edge(1, 2, weight=5)
    # G.add_edge(1, 3, weight=4)
    F = nx.gomory_hu_tree(G, capacity="weight")
    drawDirectedNetwork(F, "gomoryhu-2")
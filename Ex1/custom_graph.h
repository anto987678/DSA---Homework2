#ifndef CUSTOM_GRAPH_H
#define CUSTOM_GRAPH_H

#include "custom_list.h"

template <typename EdgeInfo>
struct GraphEdge {
    int target;
    EdgeInfo value; //can be a digit for this homework
};

//used graph from labs
template <typename NodeInfo, typename EdgeInfo>
class CustomGraph {
public:
    int nodeCount;
    NodeInfo* nodeValues;
    CustomList<GraphEdge<EdgeInfo>>* connections;

    CustomGraph(int n) {
        nodeCount = n;
        nodeValues = new NodeInfo[n];
        connections = new CustomList<GraphEdge<EdgeInfo>>[n];
    }

    void setNodeValue(int node, NodeInfo val) {
        nodeValues[node] = val;
    }

    NodeInfo getNodeValue(int node) const {
        return nodeValues[node];
    }

    void addConnection(int from, int to) {
        GraphEdge<EdgeInfo> e;
        e.target = to;
        connections[from].append(e);
    }

    void setEdgeValue(int from, int to, EdgeInfo val) {
        auto curr = connections[from].getFirst();
        while (curr) {
            if (curr->data.target == to) {
                curr->data.value = val;
                return;
            }
            curr = curr->next;
        }
    }

    ~CustomGraph() {
        delete[] nodeValues;
        delete[] connections;
    }
};

#endif

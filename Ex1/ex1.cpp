#include <iostream>
#include <string>
#include <cmath>
#include "custom_graph.h"

using namespace std;

//turns a string of digits into a number (base k)
int toIndex(const string& s, int base) {
    int index = 0;
    for (char c : s) {
        index = index * base + (c - '0');
    }
    return index;
}

//build the graph, nodes are all combos of len-1
void makeGraph(CustomGraph<string, int>& g, int base, int len) {
    int totalNodes = pow(base, len - 1);
    //make node labels like 00, 01, ...
    for (int i = 0; i < totalNodes; i++) {
        string label = "";
        int val = i;
        for (int j = 0; j < len - 1; j++) {
            label = char('0' + val % base) + label;
            val /= base;
        }
        g.setNodeValue(i, label);
    }

    //add edges based on digits 0 to k-1
    for (int i = 0; i < totalNodes; i++) {
        string current = g.getNodeValue(i);
        for (int d = 0; d < base; d++) {
            string next = current.substr(1) + char('0' + d);
            int target = toIndex(next, base);
            g.addConnection(i, target);
            g.setEdgeValue(i, target, d);
        }
    }
}

//dfs to build the answer path
void dfs(CustomGraph<string, int>& g, int node, string& result) {
    while (!g.connections[node].isEmpty()) {
        auto last = g.connections[node].getLast();
        int nextNode = last->data.target;
        int digit = last->data.value;
        g.connections[node].removeLast(); //remove so we don’t revisit
        dfs(g, nextNode, result);
        result += char('0' + digit); //backtrack, build path in reverse
    }
}

int main() {
    int len, base;
    cin >> len >> base;
    //edge case
    if (len <= 0 || base <= 0) {
        cout << "bad input \n";
        return 1;
    }
    //check if k powered n is too large
    if (pow(base, len) > 4096) {
        cout << "input too large \n";
        return 1;
    }
    //case when n is 1
    if (len == 1) {
        for (int i = 0; i < base; i++) {
            cout << char('0' + i);
        }
        cout << endl;
        return 0;
    }
    int totalNodes = pow(base, len - 1);
    CustomGraph<string, int> g(totalNodes);
    makeGraph(g, base, len);
    string path = "";
    dfs(g, 0, path);
    //output is first node label + reversed path from dfs
    string start = g.getNodeValue(0);
    for (int i = 0, j = path.length() - 1; i < j; ++i, --j) 
    {
    swap(path[i], path[j]);
    }
    cout << start << path << endl;
    return 0;
}

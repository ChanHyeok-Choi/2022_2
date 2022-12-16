#include <iostream>
#include <vector>

using namespace std;

// Depth-First Search: O(n+m) for n verices, m edges
//  1. Find and report a path between two given vertices
//  2. Find a cycle in the graph

enum LABEL { UNEXPLORED, VISITED, DISCOVERY, BACK };

class Edge {
private:
    LABEL label;
    Vertice* src,* dest;
public:
    void setLabel(LABEL l) { label = l; }
    LABEL getLabel() { return label; }
    Vertice* getSrc() { return src; }
    Vertice* getDest() { return dest; }
    Vertice* opposite(Vertice* v) { return ((v == dest) ? src : dest); }
    friend class Graph;
};

class Vertice {
private:
    LABEL label;
    vector<Edge>* incidentEdges;
public:
    void setLabel(LABEL l) { label = l; }
    LABEL getLabel() { return label; }
    vector<Edge>* incidentEdges() { return incidentEdges; }
    friend class Graph;
};

class Graph {
public:
    vector<Edge> E;
    vector<Vertice> V;
};

void DFS(Graph G) {
    // Input: graph G
    // Output: labeling of the edges of G as discovery edges and back edges
    for(auto u: G.V) {
        u.setLabel(LABEL::UNEXPLORED);
    }
    for(auto e: G.E) {
        e.setLabel(LABEL::UNEXPLORED);
    }
}

void DFS(Graph G, Vertice* v) {
    // Input: graph G and a start vertex v of G
    // Output: labeling of the edges of G in the connected component of v as discovery edges and back edges
    // v.setLabel(VISITED);
    // for(auto e: *v->incidentEdges()) {
    //     if(e.getLabel() == LABEL::UNEXPLORED) {
    //         Edge w = e.opposite(v);
    //         if(w.getLabel() == LABEL::UNEXPLORED) {
    //             e.setLabel(LABEL::DISCOVERY);
    //             DFS(G, w);
    //         } else {
    //             e.setLabel(LABEL::BACK);
    //         }
    //     }
    // }
}

int main() {
    vector<Edge> E;
    vector<Vertice> V;

    return 0;
}
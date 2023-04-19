//#pragma once

#ifndef Graph_hpp
#define Graph_hpp

#include <map>
#include <iostream>
#include <queue>

template <typename V>
class Graph {
private:
    std::map<V, std::vector<std::pair<V, int>> *> *adjVertices;
public:
    Graph();
    //add the node to the graph
    void addVertex(V vertex);

    //Add edge between two nodes
    void addEdge(V vertex1, V vertex2, int edge);

    std::map<V, V> dijkstra(V source);

    void clear();

    //returns all the vertices connected to it
    std::vector<std::pair<V, int>> getNeighbors(V vertex);

    //returns the number of vertices in the graph
    int getSize();

    //prints the graph for debugging
    void printGraph();
};

#endif //Graph_HPP

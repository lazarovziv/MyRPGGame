//#pragma once

#ifndef Graph_hpp
#define Graph_hpp

#include <map>
#include <iostream>
#include <queue>
#include "GameEntity.hpp"

class Graph {
private:
    std::map<GameEntity *, std::vector<std::pair<GameEntity *, int>> *> adjVertices;
public:
    Graph();
    //add the node to the graph
    void addVertex(GameEntity * vertex);

    //Add edge between two nodes
    void addEdge(GameEntity * vertex1, GameEntity * vertex2, int edge);

    std::map<GameEntity *, int> dijkstra(GameEntity * source);

    void clear();

    //returns all the vertices connected to it
    std::vector<std::pair<GameEntity *, int>> *getNeighbors(GameEntity * vertex);

    //returns the number of vertices in the graph
    int getSize();
};

#endif //Graph_HPP

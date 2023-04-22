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
    std::vector<GameEntity *> vertices;
public:
    Graph();
    //add the node to the graph
    void addVertex(GameEntity *vertex);
    void removeVertex(GameEntity *vertex);
    //Add edge between two nodes
    void addEdge(GameEntity * vertex1, GameEntity * vertex2, int edge);

    std::map<GameEntity *, GameEntity *> dijkstra(GameEntity * source);

    void clear();

    //returns all the vertices connected to it
    std::vector<std::pair<GameEntity *, int>> *getNeighbors(GameEntity * vertex);
    std::vector<GameEntity *> getVertices();

    //returns the number of vertices in the graph
    int getSize();

    void printPath(const std::map<GameEntity *, GameEntity *>& path);
};

#endif //Graph_HPP

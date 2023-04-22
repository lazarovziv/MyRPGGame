//#pragma once

#ifndef Graph_hpp
#define Graph_hpp

#include <map>
#include <iostream>
#include <queue>
#include "GameEntity.hpp"

template<typename T1> struct greaterPair {
    constexpr bool operator()(const std::pair<T1, int> &first, const std::pair<T1, int> &second) const {
        return first.second > second.second;
    }
};

template <class V>
class Graph {
private:
//    std::map<GameEntity *, std::vector<std::pair<GameEntity *, int>> *> adjVertices;
//    std::vector<GameEntity *> vertices;

    std::map<V, std::vector<std::pair<V, int>> *> genericGraph;
    std::vector<V> genericVertices;
public:
    Graph() = default;
    //add the node to the graph
//    void addVertex(GameEntity *vertex);
    void addVertex(V vertex) {
        genericGraph[vertex] = new std::vector<std::pair<V, int>>();
        genericVertices.push_back(vertex);
    }
//    void removeVertex(GameEntity *vertex);
    void removeVertex(V vertex) {
//    auto iterator = adjVertices.find(vertex);
        genericGraph.erase(vertex);
        genericVertices.erase(std::find(genericVertices.begin(), genericVertices.end(), vertex));
    }

    bool isInGraph(V vertex) {
        return genericGraph[vertex] != nullptr;
    }
    //Add edge between two nodes
//    void addEdge(GameEntity *vertex1, GameEntity *vertex2, int edge);
    void addEdge(V vertex1, V vertex2, int edge) {
        genericGraph[vertex1]->push_back(make_pair(vertex2, edge));
    }

//    std::map<GameEntity *, GameEntity *> dijkstra(GameEntity * source);
    std::map<V, V> dijkstra(V source) {
        std::map<V, int> distances;
        std::map<V, V> previous;
        std::map<V, bool> visited;
        std::priority_queue<std::pair<V, int>, std::vector<std::pair<V, int>>, greaterPair<V>> queue;
        V u, v;
        int weight = 0;
        int countVisited = 0;
        int verticesSize = getSize();
        int alt = INT32_MAX;

        distances[source] = 0;
        for (auto &item : genericGraph) {
            if (item.first != source) {
                distances[item.first] = INT32_MAX;
                visited[item.first] = false;
            }
            queue.emplace(item.first, distances[item.first]);
        }
        visited[source] = true;
        countVisited++;

        while (countVisited != verticesSize) {
            u = queue.top().first;
            queue.pop();

            for (int i = 0; i < genericGraph[u]->size(); i++) {
                v = genericGraph[u]->at(i).first;
                weight = genericGraph[u]->at(i).second;
                alt = distances[u] + weight;

                if (alt < distances[v]) {
                    distances[v] = alt;
                    previous[v] = u;
                    queue.emplace(v, alt);
                }
            }
            countVisited++;
        }

        return previous;
    }

    void clear() {
//        adjVertices.clear();
//        vertices.clear();
        genericGraph.clear();
        genericVertices.clear();
    }

    //returns all the vertices connected to it
//    std::vector<std::pair<GameEntity *, int>> *getNeighbors(GameEntity *vertex);
    std::vector<std::pair<V, int>> *getNeighbors(V vertex) {
        return genericGraph[vertex];
    }
//    std::vector<GameEntity *> getVertices();
    std::vector<V> getVertices() {
        return genericVertices;
//        return vertices;
    }

    //returns the number of vertices in the graph
//    int getSize();
    int getSize() {
        return genericGraph.size();
    }

//    void printPath(const std::map<GameEntity *, GameEntity *>& path);
//    void printPath(const std::map<V, V>& path);
};

#endif //Graph_HPP

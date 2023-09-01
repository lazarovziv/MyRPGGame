//#pragma once

#ifndef Graph_hpp
#define Graph_hpp

#include <map>
#include <set>
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
    std::map<V, std::vector<std::pair<V, int>> *> genericGraph;
    std::vector<V> genericVertices;
public:
    Graph() = default;
    // add the node to graph
    void addVertex(V vertex) {
        genericGraph[vertex] = new std::vector<std::pair<V, int>>();
        genericVertices.push_back(vertex);
    }
    // remove node from graph
    void removeVertex(V vertex) {
        if (!isInGraph(vertex)) return;
        std::pair<V, int> vertexPair = std::make_pair(vertex, 1);
        // need to delete all edges with neighbors of vertex
        std::pair<V, int> pair;
        // neighbors of vertex
        for (int i = 0; i < genericGraph[vertex]->size(); i++) {
            pair = genericGraph[vertex]->at(i);
            // deleting edges TO vertex
            genericGraph[pair.first]->erase(
                    std::find(genericGraph[pair.first]->begin(),
                              genericGraph[pair.first]->end(),
                              vertexPair));
        }
        // deleting edges FROM vertex
        genericGraph.erase(vertex);
        genericVertices.erase(std::find(genericVertices.begin(), genericVertices.end(), vertex));
    }

    // is node in graph
    bool isInGraph(V vertex) {
        return genericGraph[vertex] != nullptr;
    }
    // add edge between two nodes with its weight
    void addEdge(V vertex1, V vertex2, int edge) {
        genericGraph[vertex1]->push_back(make_pair(vertex2, edge));
    }

    int h(V first, V second) {
        return first - second;
    }

    std::vector<V> *reconstructPath(std::map<V, V> cameFrom, V current) {
        auto *totalPath = new std::vector<V>();
        while (current != nullptr) {
            current = cameFrom[current];
            totalPath->push_back(current);
        }
        return totalPath;
    }

    // A* algorithm
    std::vector<V> *findPathTo(V source, V target) {
        std::priority_queue<std::pair<V, float>, std::vector<std::pair<V, float>>, greaterPair<V>> openQueue;
        std::set<V> openSet;
        std::map<V, V> cameFrom;
        std::map<V, float> gScore;
        std::map<V, float> fScore;
        std::pair<V, float> current, neighbor;
        float tentativeGScore;
        cameFrom[source] = nullptr;

        for (auto &item : genericGraph) {
            if (item.first != source) {
                gScore[item.first] = std::numeric_limits<float>::max();
                fScore[item.first] = std::numeric_limits<float>::max();
            }
        }

        gScore[source] = 0;
        fScore[source] = 0;

        // inserting source with distance from itself as 0 to queue
        openQueue.emplace(source, fScore[source]);
        openSet.insert(source);

        while (!openSet.empty()) {
            current = openQueue.top();
            openQueue.pop();
            openSet.erase(current.first);
            // reached goal
            if (current.first == target) return reconstructPath(cameFrom, current.first);

            for (int i = 0; i < genericGraph[current.first]->size(); i++) {
                neighbor = genericGraph[current.first]->at(i);
                tentativeGScore = gScore[current.first] + neighbor.second; // second is the weight of edge (current, generic[current]->at(i).first)

                // if path is better
                if (tentativeGScore < gScore[neighbor.first]) {
                    cameFrom[neighbor.first] = current.first;
                    gScore[neighbor.first] = tentativeGScore;
                    fScore[neighbor.first] = tentativeGScore + h(source, neighbor.first);

                    if (std::find(openSet.begin(), openSet.end(), neighbor.first) == openSet.end()) {
                        openQueue.emplace(neighbor.first, gScore[neighbor.first]);
                        openSet.insert(neighbor.first);
                    }
                }
            }
        }

        return nullptr;
    }

    // dijkstra algorithm from source node
    std::map<V, V> dijkstra(V source) {
        std::map<V, int> distances;
        std::map<V, V> previous;
        std::map<V, bool> visited;
        std::priority_queue<std::pair<V, int>, std::vector<std::pair<V, int>>, greaterPair<V>> queue;
        std::priority_queue<std::pair<V,int>, std::vector<std::pair<V, int>>, greaterPair<V>> tempQueue;
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

                    // reinserting v to the queue
                    while (!queue.empty()){
                        auto item = queue.top();
                        queue.pop();
                        if(item.first == v) tempQueue.push(std::make_pair(v, alt));
                        else tempQueue.push(item);
                    }
                    // emptying temp queue and reinserting updated values to queue
                    while (!tempQueue.empty()) {
                        queue.push(tempQueue.top());
                        tempQueue.pop();
                    }
                    // what was before
//                    queue.emplace(v, alt);
                }
            }
            countVisited++;
        }
        return previous;
    }

    // clears graph
    void clear() {
        genericGraph.clear();
        genericVertices.clear();
    }

    // returns all the vertices connected to given node
    std::vector<std::pair<V, int>> *getNeighbors(V vertex) {
        return genericGraph[vertex];
    }

    std::vector<V> getVertices() {
        return genericVertices;
    }

    // returns the number of vertices in the graph
    int getSize() {
        return genericVertices.size();
    }
};

#endif //Graph_HPP

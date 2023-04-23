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
        genericGraph.erase(vertex);
        genericVertices.erase(std::find(genericVertices.begin(), genericVertices.end(), vertex));
    }

    // is node in graph
    bool isInGraph(V vertex) {
        return genericGraph[vertex] != nullptr;
    }
    // add edge between two nodes with its' weight
    void addEdge(V vertex1, V vertex2, int edge) {
        genericGraph[vertex1]->push_back(make_pair(vertex2, edge));
    }

    int h(V first, V second) {
        return first - second;
    }

    std::vector<V> *reconstructPath(std::map<V, V> cameFrom, V current) {
        std::vector<V> *totalPath = new std::vector<V>();
        while (isInGraph(current)) {
            current = cameFrom[current];
            totalPath->push_back(current);
        }
        return totalPath;
    }

    // A* algorithm
    std::vector<V> *findPathTo(V source, V target) {
        std::priority_queue<std::pair<V, int>, std::vector<std::pair<V, int>>, greaterPair<V>> openQueue;
        std::set<V> openSet;
        std::map<V, V> cameFrom;
        std::map<V, int> gScore;
        std::map<V, int> fScore;
        std::pair<V, int> current, neighbor;
        int tentativeGScore;

        // inserting source with distance from itself as 0 to queue
        openQueue.emplace(source, 0);
        openSet.insert(source);
        for (auto &item : genericGraph) {
            if (item.first != source) {
                gScore[item.first] = INT32_MAX;
                fScore[item.first] = INT32_MAX;
            }
        }

        gScore[source] = 0;
        fScore[source] = 0;

        while (!openQueue.empty()) {
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

                    if (std::find(openSet.begin(), openSet.end(), neighbor.first) != openSet.end()) {
                        openQueue.emplace(neighbor.first, gScore[neighbor.first]);
                        openSet.insert(neighbor.first);
                    }
                }
            }
        }

        return nullptr;
    }

    // dijkstra algorithm from source node (TODO: add function for specific target)
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
        return genericGraph.size();
    }
};

#endif //Graph_HPP

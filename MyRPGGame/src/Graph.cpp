#include "../include/Graph.hpp"

template<typename T1> struct greaterPair {
    constexpr bool operator()(const std::pair<T1, int> &first, const std::pair<T1, int> &second) const {
        return first.second > second.second;
    }
};

template <typename V>
Graph<V>::Graph() {
    adjVertices = new std::map<V, std::vector<std::pair<V, int>> *>();
};

template <typename V>
void Graph<V>::addVertex(V vertex) {
    *(adjVertices)[vertex] = new std::vector<std::pair<V, int>>();
}

template <typename V>
void Graph<V>::addEdge(V vertex1, V vertex2, int edge) {
    *(adjVertices)[vertex1]->push_back(make_pair(vertex2, edge));
}

template <typename V>
std::map<V, V> Graph<V>::dijkstra(V source) {
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
    for (auto &item : *adjVertices) {
        if (item.first != source) {
            distances[item.first] = INT32_MAX;
            visited[item.first] = false;
        }
        queue.push(item.first);
    }
    visited[source] = true;
    countVisited++;

    while (countVisited != verticesSize) {
        u = queue.top().first;
        queue.pop();
        for (int i = 0; i < *(adjVertices)[u].size(); i++) {
            v = *(adjVertices)[u][i].first;
            weight = *(adjVertices)[u][i].second;
            alt = distances[u] + weight;

            if (alt < distances[v]) {
                distances[v] = alt;
                previous[v] = u;
                queue.push(make_pair(v, alt));
            }
        }
    }

    return previous;
}

template <typename V>
void Graph<V>::clear() {
    adjVertices->clear();
}

template <typename V>
std::vector<std::pair<V, int>> Graph<V>::getNeighbors(V vertex) {
    return *(adjVertices)[vertex];
}

template <typename V>
int Graph<V>::getSize() {
    return adjVertices->size();
}

template <typename V>
void Graph<V>::printGraph() {
    for (auto i : adjVertices) {
        for (auto j : i.second) {
            std::cout << i.first << "->" << j << std::endl;
        }
    }
}
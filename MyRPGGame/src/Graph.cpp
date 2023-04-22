#include "../include/Graph.hpp"

template<typename T1> struct greaterPair {
    constexpr bool operator()(const std::pair<T1, int> &first, const std::pair<T1, int> &second) const {
        return first.second > second.second;
    }
};

Graph::Graph() = default;

void Graph::addVertex(GameEntity *vertex) {
    adjVertices[vertex] = new std::vector<std::pair<GameEntity *, int>>();
    vertices.push_back(vertex);
}

void Graph::removeVertex(GameEntity *vertex) {
//    auto iterator = adjVertices.find(vertex);
    adjVertices.erase(vertex);
    vertices.erase(std::find(vertices.begin(), vertices.end(), vertex));
}

void Graph::addEdge(GameEntity * vertex1, GameEntity * vertex2, int edge) {
    adjVertices[vertex1]->push_back(make_pair(vertex2, edge));
}

std::map<GameEntity *, GameEntity *> Graph::dijkstra(GameEntity * source) {
    std::map<GameEntity *, int> distances;
    std::map<GameEntity *, GameEntity *> previous;
    std::map<GameEntity *, bool> visited;
    std::priority_queue<std::pair<GameEntity *, int>, std::vector<std::pair<GameEntity *, int>>, greaterPair<GameEntity *>> queue;
    GameEntity *u, *v;
    int weight = 0;
    int countVisited = 0;
    int verticesSize = getSize();
    int alt = INT32_MAX;

    distances[source] = 0;
    for (auto &item : adjVertices) {
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

        for (int i = 0; i < adjVertices[u]->size(); i++) {
            v = adjVertices[u]->at(i).first;
            weight = adjVertices[u]->at(i).second;
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

void Graph::clear() {
    adjVertices.clear();
    vertices.clear();
}

std::vector<std::pair<GameEntity *, int>> *Graph::getNeighbors(GameEntity *vertex) {
    return adjVertices[vertex];
}

std::vector<GameEntity *> Graph::getVertices() {
    return vertices;
}

int Graph::getSize() {
    return adjVertices.size();
}

void Graph::printPath(const std::map<GameEntity *, GameEntity *>& path) {
    for (auto &pair : path) {
        cout << "(" << pair.second->getPosition().x << ", " << pair.second->getPosition().y << ") -> (" << pair.first->getPosition().x << ", " << pair.first->getPosition().y << ")" << endl;
    }
}
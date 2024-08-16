#include "../include/Graph.hpp"

//template<typename T1> struct greaterPair {
//    constexpr bool operator()(const std::pair<T1, int> &first, const std::pair<T1, int> &second) const {
//        return first.second > second.second;
//    }
//};

//template <class V>
//Graph<V>::Graph() = default;

//template <class V>
//void Graph<V>::addVertex(GameEntity *vertex) {
//    adjVertices[vertex] = new std::vector<std::pair<GameEntity *, int>>();
//    vertices.push_back(vertex);
//}

//template <class V>
//void Graph<V>::addVertex(V vertex) {
//    genericGraph[vertex] = new std::vector<std::pair<V, int>>();
//    genericVertices.push_back(vertex);
//}

//template <class V>
//void Graph<V>::removeVertex(GameEntity *vertex) {
////    auto iterator = adjVertices.find(vertex);
//    adjVertices.erase(vertex);
//    vertices.erase(std::find(vertices.begin(), vertices.end(), vertex));
//}

//template <class V>
//void Graph<V>::removeVertex(V vertex) {
////    auto iterator = adjVertices.find(vertex);
//    genericGraph.erase(vertex);
//    genericVertices.erase(std::find(genericVertices.begin(), genericVertices.end(), vertex));
//}

//template <class V>
//void Graph<V>::addEdge(GameEntity * vertex1, GameEntity * vertex2, int edge) {
//    adjVertices[vertex1]->push_back(make_pair(vertex2, edge));
//}

//template <class V>
//void Graph<V>::addEdge(V vertex1, V vertex2, int edge) {
//    genericGraph[vertex1]->push_back(make_pair(vertex2, edge));
//}

//template <class V>
//std::map<GameEntity *, GameEntity *> Graph<V>::dijkstra(GameEntity * source) {
//    std::map<GameEntity *, int> distances;
//    std::map<GameEntity *, GameEntity *> previous;
//    std::map<GameEntity *, bool> visited;
//    std::priority_queue<std::pair<GameEntity *, int>, std::vector<std::pair<GameEntity *, int>>, greaterPair<GameEntity *>> queue;
//    GameEntity *u, *v;
//    int weight = 0;
//    int countVisited = 0;
//    int verticesSize = getSize();
//    int alt = INT32_MAX;
//
//    distances[source] = 0;
//    for (auto &item : adjVertices) {
//        if (item.first != source) {
//            distances[item.first] = INT32_MAX;
//            visited[item.first] = false;
//        }
//        queue.emplace(item.first, distances[item.first]);
//    }
//    visited[source] = true;
//    countVisited++;
//
//    while (countVisited != verticesSize) {
//        u = queue.top().first;
//        queue.pop();
//
//        for (int i = 0; i < adjVertices[u]->size(); i++) {
//            v = adjVertices[u]->at(i).first;
//            weight = adjVertices[u]->at(i).second;
//            alt = distances[u] + weight;
//
//            if (alt < distances[v]) {
//                distances[v] = alt;
//                previous[v] = u;
//                queue.emplace(v, alt);
//            }
//        }
//        countVisited++;
//    }
//
//    return previous;
//}

//template <class V>
//std::map<V, V> Graph<V>::dijkstra(V source) {
//    std::map<V, int> distances;
//    std::map<V, V> previous;
//    std::map<V, bool> visited;
//    std::priority_queue<std::pair<V, int>, std::vector<std::pair<V, int>>, greaterPair<V>> queue;
//    V *u, *v;
//    int weight = 0;
//    int countVisited = 0;
//    int verticesSize = getSize();
//    int alt = INT32_MAX;
//
//    distances[source] = 0;
//    for (auto &item : genericGraph) {
//        if (item.first != source) {
//            distances[item.first] = INT32_MAX;
//            visited[item.first] = false;
//        }
//        queue.emplace(item.first, distances[item.first]);
//    }
//    visited[source] = true;
//    countVisited++;
//
//    while (countVisited != verticesSize) {
//        u = queue.top().first;
//        queue.pop();
//
//        for (int i = 0; i < genericGraph[u]->size(); i++) {
//            v = genericGraph[u]->at(i).first;
//            weight = genericGraph[u]->at(i).second;
//            alt = distances[u] + weight;
//
//            if (alt < distances[v]) {
//                distances[v] = alt;
//                previous[v] = u;
//                queue.emplace(v, alt);
//            }
//        }
//        countVisited++;
//    }
//
//    return previous;
//}

//template <class V>
//void Graph<V>::clear() {
//    adjVertices.clear();
//    vertices.clear();
//    genericGraph.clear();
//    genericVertices.clear();
//}

//template <class V>
//std::vector<std::pair<GameEntity *, int>> *Graph<V>::getNeighbors(GameEntity *vertex) {
//    return adjVertices[vertex];
//}

//template <class V>
//std::vector<std::pair<V, int>> *Graph<V>::getNeighbors(V vertex) {
//    return genericGraph[vertex];
//}

//template <class V>
//std::vector<V> Graph<V>::getVertices() {
//    return vertices;
//}

//template <class V>
//std::vector<V> Graph<V>::getGenericVertices() {
//    return genericVertices;
//}

//template <class V>
//int Graph<V>::getSize() {
//    return genericGraph.size();
//}

//template <class V>
//int Graph<V>::getGenericSize() {
//    return genericGraph.size();
//}

//template <class V>
//void Graph<V>::printPath(const std::map<GameEntity *, GameEntity *>& path) {
//    for (auto &pair : path) {
//        cout << "(" << pair.second->getPosition().x << ", " << pair.second->getPosition().y << ") -> (" << pair.first->getPosition().x << ", " << pair.first->getPosition().y << ")" << endl;
//    }
//}

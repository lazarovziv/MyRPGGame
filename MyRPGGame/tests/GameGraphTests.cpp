#include <iostream>
#include "../main/include/Game.hpp"


int main() {
    Player player(PlayerType::DUAL_WIELDER);
    NPCEnemy enemy0(0, 0, 0);
    NPCEnemy enemy1(0, 64, 0);
    NPCEnemy enemy2(0, 0, 64);
    NPCEnemy enemy3(0, 32, 32);

    Node playerNode;
    Node enemy0Node;
    Node enemy1Node;
    Node enemy2Node;
    Node enemy3Node;

    playerNode.entity = &player;
    enemy0Node.entity = &enemy0;
    enemy1Node.entity = &enemy1;
    enemy2Node.entity = &enemy2;
    enemy3Node.entity = &enemy3;

    GameGraph graph(&player);

    graph.insertNode(&playerNode);
    graph.insertNode(&enemy0Node);
    graph.insertNode(&enemy1Node);
    graph.insertNode(&enemy2Node);
    graph.insertNode(&enemy3Node);

    graph.addNodeTo(&playerNode, &enemy3Node);
    graph.addNodeTo(&playerNode, &enemy2Node);

    graph.addNodeTo(&enemy3Node, &enemy2Node);
    graph.addNodeTo(&enemy3Node, &enemy1Node);
    graph.addNodeTo(&enemy1Node, &enemy0Node);
    graph.addNodeTo(&enemy2Node, &enemy0Node);

    vector<pair<int, int> > path = graph.findPathTo(&playerNode, &enemy0Node);

    for (int i = 0; i < path.size(); i++) {
        cout << "(" << path.at(i).first << ", " << path.at(i).second << ")" << endl;
    }
}

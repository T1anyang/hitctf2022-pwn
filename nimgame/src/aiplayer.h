#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "game.h"
#include <list>

struct tree_node
{
    int value = 0;
    int lowest;
    int highest;
    int depth = 0;
    list<struct tree_node*> *children = nullptr;
};

struct tree_node* create_node();
void add_child(struct tree_node* parent, struct tree_node* child);
void delete_child(struct tree_node* parent, int index);
void delete_node(struct tree_node* node);

class GameTreeAIPlayer: public Player{
protected:
    int most_serch_depth;
    Action* min_max(struct tree_node* node, Board* board);
public:
    Action* move();
};

#endif
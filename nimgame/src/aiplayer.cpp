#include "aiplayer.h"
#include <stdlib.h>

struct tree_node* create_node(){
    struct tree_node* node = (struct tree_node*) malloc(sizeof(struct tree_node));
    node->lowest = INT32_MIN;
    node->highest = INT32_MAX;
    node->children = new list<struct tree_node*>();
    return node;
}

void add_child(struct tree_node* parent, struct tree_node* child){
    parent->children->push_back(child);
    child->depth = parent->depth + 1;
}

void delete_node(struct tree_node* node){
    while(!node->children->empty()){
        struct tree_node* child = node->children->back();
        delete_node(child);
        node->children->pop_back();
    }
    delete node->children;
    free(node);
}

void delete_child(struct tree_node* parent, int index){
    list<struct tree_node*>::iterator it = parent->children->begin();
    advance(it, index);
    delete_node(*it);
    parent->children->erase(it);
}

Action* GameTreeAIPlayer::min_max(struct tree_node* node, Board* board){
    int is_max_layer = !(node->depth % 2);
    if(is_max_layer){
        node->value = INT32_MIN;
    }
    else{
        node->value = INT32_MAX;
    }
    Action* best_action = nullptr;
    list<Action*> *actions = board->get_legal_actions();
    if(board->get_winner() || (most_serch_depth > 0 && node->depth >= most_serch_depth) || actions->empty()){ // end of search
        node->value = board->envaluation(id);
        return nullptr;
    }
    for(list<Action*>::iterator itor = actions->begin(); itor != actions->end(); itor++){
        struct tree_node* child = create_node();
        add_child(node, child);
        board->move(*itor);
        if(is_max_layer){ // max
            if(node->value >= node->highest){ // alpha-beta cut branch
                board->unmove(*itor);
                break;
            }
            child->lowest = node->value;
            min_max(child, board);
            best_action = node->value > child->value? best_action : *itor;
            node->value = max(node->value, child->value);
        }
        else{ // min
            if(node->value <= node->lowest){ // alpha-beta cut branch
                board->unmove(*itor);
                break;
            }
            child->highest = node->value;
            min_max(child, board);
            best_action = node->value < child->value? best_action : *itor;
            node->value = min(node->value, child->value);
        }
        board->unmove(*itor);
    }
    return best_action;
}

Action* GameTreeAIPlayer::move(){
    // game tree. min max search. alpha-beta cut branch
    printf("%s's turn!\n", this->name);
    struct tree_node *root = create_node();
    root->depth = 0;
    Action* action = min_max(root, game->get_board());
    delete_node(root);
    return action;
}
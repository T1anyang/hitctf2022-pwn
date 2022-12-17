#ifndef NIMGAME_H
#define NIMGAME_H

#include "game.h"
#include "aiplayer.h"

class NimAction: public Action{
public:
    int heap_idx;
    int take_num;
    NimAction(){};
    NimAction(int heap_idx, int take_num): heap_idx(heap_idx), take_num(take_num){};
};

class NimBoard: public Board{
private:
    char* hall_of_fame[4];
    int heap[3];
public:
    NimBoard();
    void reset();
    void move(Action *action);
    void unmove(Action *action);
    list<Action*>* get_legal_actions();
    bool check_action_legal(Action* action);
    int get_winner();
    void show_board();
    int envaluation(int playerid);
    void leave_name();
};

class NimGame: public Game{
public:
    NimGame();
};

class NimHumanPlayer: public Player{
public:
    NimHumanPlayer(NimGame *game, int id);
    Action* move();
};

class NimAIPlayer: public GameTreeAIPlayer{
private:
    void show_flag(const char* filename);
public:
    NimAIPlayer(const char* name, NimGame* game, int id);
    NimAIPlayer(const char* name, NimGame* game, int id, int most_search_depth);
    void on_success();
    void on_failure();
};

#endif
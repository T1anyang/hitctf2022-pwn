#ifndef GAME_H
#define GAME_H

#include<iostream>
#include<stdio.h>
#include<list>
using namespace std;

class Action{

};

class Board{
protected:
    int turn;
public:
    virtual void reset()=0;
    virtual void move(Action *action)=0;
    virtual void unmove(Action *action)=0;
    virtual list<Action*>* get_legal_actions()=0;
    virtual bool check_action_legal(Action* action)=0;
    // -1: nobody wins; 0: not over yet; 1: player1 wins; ...
    virtual int get_winner()=0;
    virtual void show_board()=0;
    virtual int envaluation(int playerid)=0;
};

class Game;

class Player{
protected:
    int id;
    char name[0x40];
    Game* game;
public:
    char* get_name();
    void set_id(int id);
    virtual Action* move()=0;
    virtual void on_success(){};
    virtual void on_failure(){};
};

class Game{
protected:
    Player *player1;
    Player *player2;
    Player *winner;
    Board *board;
    virtual void on_game_start();
    virtual void run();
    virtual void on_game_over();
public:
    Board* get_board();
    void reset_board();
    void set_p1(Player *player);
    void set_p2(Player *player);
    void start_game();
};

#endif
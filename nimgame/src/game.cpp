#include "game.h"

char* Player::get_name(){
    return this->name;
}

void Player::set_id(int id){
    this->id = id;
}

void Game::set_p1(Player* player){
    this->player1 = player;
}

void Game::set_p2(Player* player){
    this->player2 = player;
}

Board* Game::get_board(){
    return board;
}

void Game::reset_board(){
    board->reset();
}

void Game::run(){
    int turn = 0;
    board->show_board();
    Action* action = nullptr;
    while(true){
        int win = this->board->get_winner();
        if(win){
            if(win == 1){
                winner = player1;
            }
            else if(win == 2){
                winner = player2;
            }
            break;
        }
        if(!turn){
            action = player1->move();
            turn = 1;
        }
        else{
            action = player2->move();
            turn = 0;
        }
        board->move(action);
        board->show_board();
    }
}

void Game::on_game_start(){
    if (!(player1 && player2)){
        fprintf(stderr, "player not ready!\n");
        throw "player not ready!\n";
    }
    printf("%s vs %s!\n", player1->get_name(), player2->get_name());
}

void Game::on_game_over(){
    if(winner){
        printf("%s win! Congrats!\n", winner->get_name());
        if(winner == player1){
            player2->on_failure();
            player1->on_success();
        }
        else{
            player1->on_failure();
            player2->on_success();
        }
    }
    else{
        printf("no winner!\n");
    }
}

void Game::start_game(){
    on_game_start();
    run();
    on_game_over();
}
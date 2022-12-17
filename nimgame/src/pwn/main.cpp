#include "nimgame.h"
#include <unistd.h>
// g++ -no-pie -o nimgame main.cpp nimgame.cpp ../game.cpp ../aiplayer.cpp -I ..

int main(){
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    printf("The traditional game of Nim is played with a number of coins arranged in heaps: \n\
    the number of coins and heaps is randomly decided. \n\
    There are two players. \n\
    When it's a player's move he or she can take any number of coins from a single heap. \n\
    They have to take at least one coin, though, and they can't take coins from more than one heap. \n\
    The winner is the player who makes the last move, so there are no coins left after that move.\n");
    printf("====================================\n");
    char c[2] = {0};
    printf("Do you want to play with me? y/n ");
    read(0, c, 2);
    if(c[0] != 'y' && c[0] != 'Y') return 0;
    Game* game = new NimGame();
    Player *human = new NimHumanPlayer((NimGame*)game, 1);
    Player *ai = new NimAIPlayer("Computer", (NimGame*)game, 2, 4);
    do{
        char p[2];
        game->reset_board();
        printf("You want to go first or second?\n 1: you go first; 2: AI goes first\n");
        read(0, p, 2);
        if(p[0] == '1'){
            ai->set_id(2);
            human->set_id(1);
            game->set_p1(human);
            game->set_p2(ai);
        }
        else{
            ai->set_id(1);
            human->set_id(2);
            game->set_p1(ai);
            game->set_p2(human);
        }
        game->start_game();
        printf("Do you want another game? y/n ");
        read(0, c, 2);
    }while(c[0] == 'y' || c[0] == 'Y');
    printf("Bye!\n");
    return 0;
}
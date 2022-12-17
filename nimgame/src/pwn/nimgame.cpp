#include "nimgame.h"
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int readline(int fd, char* buffer, int size){
    int cnt = 0;
    while(cnt < size - 1){
        int ret = read(fd, buffer + cnt, 1);
        if(buffer[cnt] == '\n'){
            break;
        }
        if(!ret){
            break;
        }
        cnt++;
    }
    buffer[cnt] = 0;
    return cnt;
}

int read_int(){
    char buf[0x20];
    readline(0, buf, 0x18);
    return atoi(buf);
}

NimBoard::NimBoard(){
    turn = 1;
    srand(time(0));
    heap[0] = rand() % 5 + 10;
    heap[1] = rand() % 5 + 10;
    do{
        heap[2] = rand() % 5 + 10;
    }while(!(heap[0]^heap[1]^heap[2])); // guarantee that p1 have a winning strategy 
}

void NimBoard::reset(){
    turn = 1;
    srand(time(0));
    heap[0] = rand() % 5 + 10;
    heap[1] = rand() % 5 + 10;
    do{
        heap[2] = rand() % 5 + 10;
    }while(!(heap[0]^heap[1]^heap[2]));
}

void NimBoard::move(Action *action){
    NimAction *_action = (NimAction*) action;
    heap[_action->heap_idx] -= _action->take_num;
    turn = 1 + (!(turn-1));
}

void NimBoard::unmove(Action *action){
    NimAction *_action = (NimAction*) action;
    heap[_action->heap_idx] += _action->take_num;
    turn = 1 + (!(turn-1));
}

list<Action*>* NimBoard::get_legal_actions(){
    list<Action*>* actions = new list<Action*>();
    for(int i = 0; i < 3; i++){
        for(int j = 1; j <= heap[i]; j++){
            actions->push_back(new NimAction(i, j));
        }
    }
    return actions;
}

bool NimBoard::check_action_legal(Action* action){
    return ((NimAction*) action)->heap_idx >= 0
        && ((NimAction*) action)->heap_idx <= 2
        && ((NimAction*) action)->take_num > 0
        && ((NimAction*) action)->take_num <= heap[((NimAction*) action)->heap_idx];
}

int NimBoard::get_winner(){
    if(heap[0] || heap[1] || heap[2]){
        return 0;
    }
    else{
        return 1 + !(turn - 1);
    }
}

void NimBoard::show_board(){
    printf("heap A: ");
    for(int i = 0; i < heap[0]; i++){
        printf("o ");
    }
    printf("| %d left\n", heap[0]);
    printf("heap B: ");
    for(int i = 0; i < heap[1]; i++){
        printf("o ");
    }
    printf("| %d left\n", heap[1]);
    printf("heap C: ");
    for(int i = 0; i < heap[2]; i++){
        printf("o ");
    }
    printf("| %d left\n", heap[2]);
}

int NimBoard::envaluation(int playerid){
    int nimsum = heap[0] ^ heap[1] ^ heap[2];
    int inturn = playerid == turn;
    int value = (bool(nimsum) ^ bool(inturn))? INT32_MIN : INT32_MAX;
    // when AI will lose, he will take as less coins as he can.
    if(value < 0){
        value += heap[0] + heap[1] + heap[2];
    }
    return value;
}

void NimBoard::leave_name(){
    for(int i = 0; i < 4; i++){
        printf("hall of fame[%d]: %s\n", i, hall_of_fame[i]);
    }
    printf("Choose a place for yourself!\n");
    int idx;
    idx = read_int();
    char* ptr = (char*) malloc(0x90);
    if(idx >= 4){
        printf("sorry, no such place!\n");
        return;
    }
    printf("What's your name? > ");
    readline(0, ptr, 0x80);
    hall_of_fame[idx] = ptr;
    printf("Done!\n");
    for(int i = 0; i < 4; i++){
        printf("hall of fame[%d]: %s\n", i, hall_of_fame[i]);
    }
}

NimGame::NimGame(){
    board = new NimBoard();
}

NimHumanPlayer::NimHumanPlayer(NimGame* game, int id){
    this->game = game;
    this->id = id;
    printf("Your name, plz\n");
    readline(0, this->name, 0x30);
    printf("\nHello, %s!\n", this->name);
}

Action* NimHumanPlayer::move(){
    NimBoard* board = (NimBoard*) this->game->get_board();
    int idx, num;
    char heap[16] = {0};
    NimAction* action = new NimAction();
    printf("%s's turn! ", this->name);
    do{
        idx = -1; num = -1;
        printf("which heap? >");
        readline(0, heap, 3);
        idx = heap[0] - 'A';
        printf("how much to take? >");
        num = read_int();
        action->heap_idx = idx;
        action->take_num = num;
    }while(!board->check_action_legal(action) && printf("invalid choice\n"));
    return action;
}

NimAIPlayer::NimAIPlayer(const char* name, NimGame* game, int id){
    strncpy(this->name, name, 0x30);
    this->game = game;
    this->id = id;
    this->most_serch_depth = 0;
}

NimAIPlayer::NimAIPlayer(const char* name, NimGame* game, int id, int most_search_depth){
    strncpy(this->name, name, 0x30);
    this->game = game;
    this->id = id;
    this->most_serch_depth = most_search_depth;
}

void NimAIPlayer::on_success(){
    printf("Computer: Give up. You are not smart enough to beat me\n");
}

void NimAIPlayer::on_failure(){
    printf("Computer: Oh, you beat me.\n");
    printf("I will leave your name in the hall of fame on the board.\n");
    ((NimBoard*)(game->get_board()))->leave_name();
    if(id == 2){
        printf("here is your gift: ");
        show_flag("flag1");
        printf("\nCan you beat me again, if I go first?\n");
    }
    else if(id == 1){
        printf("here is your gift: ");
        show_flag("flag2");
        printf("\nYou are the god of nim\n");
    }
}

void NimAIPlayer::show_flag(const char* filename){
    char flag[0x40] = {0};
    int fd = open(filename, O_RDONLY);
    readline(fd, flag, 0x40);
    printf("%s\n", flag);
}
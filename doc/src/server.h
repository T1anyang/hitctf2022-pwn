#ifndef SERVER
#define SERVER

#include "doc.h"

struct docsrc{
    char mode;
    char* name;
};

#define MAX_FILE 10
#define TEXT_MAX_LEN 2048

#define INDEX           0
#define LS              1
#define EXTRACT         2
#define EXTRACT_MANY    3
#define UPLOAD          4
#define NOT_FOUND       5

struct mem{
    char utf8text[TEXT_MAX_LEN];
    struct docsrc docs[MAX_FILE];
    int cnt;
};

int home(char* query);
int ls(char* query);
int extract(char* query);
int extract_many(char* query);
int upload(char* query);
int not_found(char* query);

void response(int code, char* title, char* msg);

#endif
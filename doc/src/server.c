#include "server.h"
#include "dirent.h"

struct mem bssmem;


int (*apis[])(char* query) = {
    home,
    ls,
    extract,
    extract_many,
    upload,
    not_found,
};

void initial(){
    bssmem.docs[0].mode = 0;
    bssmem.docs[0].name = "flag.doc";
    bssmem.docs[1].mode = 1;
    bssmem.docs[1].name = "help.doc";
    bssmem.docs[2].mode = 1;
    bssmem.docs[2].name = "poem_en.doc";
    bssmem.docs[3].mode = 1;
    bssmem.docs[3].name = "poem_cn.doc";
    bssmem.cnt = 0;
    DIR* dir = opendir("./src");
    struct dirent* in_file;
    bssmem.cnt = 4;
    while(in_file = readdir(dir)){
        if(!strncmp(in_file->d_name, "user", 4)){
            bssmem.docs[bssmem.cnt].mode = 1;
            bssmem.docs[bssmem.cnt].name = (char*) malloc(strlen(in_file->d_name) + 8);
            strcpy(bssmem.docs[bssmem.cnt].name, in_file->d_name);
            bssmem.cnt++;
        }
    }
    bssmem.cnt -= 4;
}

int route_api(){
    char* path = getenv("REQUEST_URI");
    char* query = strstr(path, "?");
    if(!query){
        query = path + strlen(path);
    }
    char buffer[128] = {0};
    strncpy(buffer, path, query - path);
    if(!strcmp(buffer, "/doc") || !strcmp(buffer ,"/doc/")){
        return INDEX;
    }
    if(!strcmp(buffer, "/doc/ls") || !strcmp(buffer ,"/doc/ls/")){
        return LS;
    }
    if(!strcmp(buffer, "/doc/extract") || !strcmp(buffer ,"/doc/extract/")){
        return EXTRACT;
    }
    if(!strcmp(buffer, "/doc/extract_many") || !strcmp(buffer ,"/doc/extract_many/")){
        return EXTRACT_MANY;
    }
    if(!strcmp(buffer, "/doc/upload") || !strcmp(buffer ,"/doc/upload/")){
        return UPLOAD;
    }

    return NOT_FOUND;
}

int main(int argc, char* argv[]){
    initial();
    int (*api)(char* query) = apis[route_api()];
    int ret = api(getenv("QUERY_STRING"));
    return 0;
}

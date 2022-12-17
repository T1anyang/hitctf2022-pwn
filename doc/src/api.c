#include "server.h"

extern struct mem bssmem;

void response(int code, char* title, char* msg){
    printf("Status: %d\r\n", code);
    printf("Content-type: text/html\r\n\r\n");
    printf("<head><meta charset=\"UTF-8\"><title>%s</title><head>", title);
    puts(msg);
}

int home(char* query){
    response(200, "home",
    "<h1>HITCTF 2022</h1>" \
    "<p>Welcome to use my microsoft doc extractor</p>" \
    "<p>Give me a Microsoft word 2003 (.doc) format file, and you can get the text in the file.</p>" \
    "<p>Supported language: English & Chinese</p>" \
    "<p><a href='/doc/ls'>See file list</a></p>" \
    "<p><a href='/doc/upload'>Upload a file</a></p>" \
    );
    return 0;
}

int ls(char* query){
    response(200, "ls", "");
    printf("<ul>");
    for(int i = 0; i < MAX_FILE; i++){
        if(!bssmem.docs[i].name){
            break;
        }
        char* access = bssmem.docs[i].mode? "ALLOW": "FORBIDDEN";
        printf("<li>name:<a href=\"/doc/extract?file=%s\">%s</a> mode:%s</li>", bssmem.docs[i].name, bssmem.docs[i].name, access);
    }
    printf("</ul>");
    return 0;
}

int extract(char* query){
    char filepath[64];
    char* filename = strstr(query, "file=");
    if(!filename){
        response(400, "extract", "<h1>give file name!</h1>");
        return -1;
    }
    filename += 5;
    int i = 0;
    for(; i < MAX_FILE; i++){
        if(!bssmem.docs[i].name || !strcmp(filename, bssmem.docs[i].name)){
            break;
        }
    }
    if(i == MAX_FILE || !bssmem.docs[i].name){
        response(404, "extract", "<h1>file not found!</h1>");
        return -1;
    }
    if(!bssmem.docs[i].mode){
        response(403, "extract", "<h1>file forbidden!</h1>");
        return -1;
    }
    snprintf(filepath, 60, "./src/%s", filename);
    struct doc_file* doc = doc_init(filepath);
    if(!doc){
        response(400, "extract", "<h1>bad file!</h1>");
        return -1;
    }
    char* text = bssmem.utf8text;
    size_t size = get_utf8_text(doc, text, TEXT_MAX_LEN);
    response(200, filename, "");
    printf("<h2>%s</h2>", bssmem.docs[i].name);
    puts("<pre style='font-size:18px;'>");
    puts(text);
    puts("</pre>");
    doc_clear(doc);
}

int extract_many(char* query){
    char filepath[64];
    char filename[32];
    char* filenames = strstr(query, "files=");
    if(!filenames){
        response(400, "extract_many", "<h1>give file names!</h1>");
        return -1;
    }
    response(200, "extract_many", "");
    filenames += 6;
    char* begin = filenames;
    char* end = index(filenames, '|');
    if(!end){
        end = begin + strlen(filenames);
    }
    while(begin != end){
        memset(filename, 0, 32);
        memset(filepath, 0, 64);
        int len = 30 > end - begin? end - begin: 30;
        strncpy(filename, begin, len);
        filename[len] = 0;
        int i = 0;
        printf("<h2>%s</h2>", filename);
        for(; i < MAX_FILE; i++){
            if(!bssmem.docs[i].name || !strcmp(filename, bssmem.docs[i].name)){
                break;
            }
        }
        if(i == MAX_FILE || !bssmem.docs[i].name){
            printf("<p>file not found!<p>");
            goto next;
        }
        if(!bssmem.docs[i].mode){
            printf("<p>file forbidden!</p>");
            goto next;
        }

        snprintf(filepath, 60, "./src/%s", filename);
        struct doc_file* doc = doc_init(filepath);
        if(!doc){
            printf("<p>bad file!</p>");
            doc_clear(doc);
            goto next;
        }
        char* text = bssmem.utf8text;
        memset(text, 0, TEXT_MAX_LEN);
        size_t size = get_utf8_text(doc, text, TEXT_MAX_LEN);
        puts("<pre style='font-size:18px;'>");
        puts(text);
        puts("</pre>");
        doc_clear(doc);
    next:
        if(*end == 0){
            begin = end;
        }
        else{
            begin = end + 1;
            end = index(begin, '|');
            if(!end){
                end = begin + strlen(begin);
            }
        }
    }
    return 0;
}

int upload(char* query){
    if(!strcmp(getenv("REQUEST_METHOD"), "GET")){
        response(200, "upload",
        "<form id=\"upload-form\" > \
            <input type=\"file\" id=\"upload\" name=\"upload\"/> <br /> \
        </form>" \
        "<script>var filecontent;function upload_doc(){let reader = new FileReader(); reader.onload = function(e) {filecontent = e.target.result; console.log(filecontent); fetch('/doc/upload', {method: 'POST', body:filecontent}).then(res => {res.text().then(s => {document.getElementById('comment').innerHTML=s})}); };reader.readAsArrayBuffer(document.getElementById('upload').files[0]);}</script>" \
        "<button onclick='upload_doc()'>UPLOAD</button>" \
        "<div id='comment'></div>"
        );
        return 0;
    }
    if(bssmem.cnt + 4 >= MAX_FILE){
        response(400, "upload", 
        "<h1>too many files!</h1>");
        return -1;
    }
    char buffer[512];
    char filename[64] = {0};
    snprintf(filename, 60, "./src/user%d.doc", bssmem.cnt);
    FILE* saved = fopen(filename, "wb");
    size_t size;
    while(size = fread(buffer, 1, 512, stdin)){
        fwrite(buffer, 1, size, saved);
    }
    response(200, "upload", "");
    printf("Your file: user%d.doc", bssmem.cnt);
    fclose(saved);
    bssmem.cnt++;
    return 0;
}

int not_found(char* query){
    response(404, "not found", 
    "<h1>page not found!</h1>"
    );
    return 0;
}
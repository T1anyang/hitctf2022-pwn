#ifndef DOC
#define DOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOC_HEADER_LEN 512
#define END_OF_CHAIN -2

#define UNKNOWN 0
#define WORD_ASCII 1
#define CN_UTF16 2
#define NOTTEXT -1

// Header of a .doc file. DO NOT change
struct doc_header
{
    char magic[8]; // 0-8
    char uid[16]; // 8-24
    unsigned short revision; // 24-26
    unsigned short vision; // 26-28
    unsigned short byte_order; // 28-30
    unsigned short ssz; // 30-32 sec_size = 2^ssz
    unsigned short sssz; // 32-34 short_sec_size = 2^sssz
    char unused0[10]; // 34-44
    int sat_secnum; // 44-48
    int dir_stream_secid; // 48-52
    char unused1[4]; // 52-56
    int std_stream_minsize; //56-60 
    int ssat_secid; // 60-64
    int ssat_secnum; // 64-68
    int msat_secid; // 68-72
    int msat_secnum; // 72-76
    int msat[109]; // 76-512
};

struct secids
{
    struct secids *next;
    int ids[128];
};

// directory in a .doc file. DO NOT change
struct directory
{
    char name[64];
    short namelen;
    char type;
    char color; // red-black tree node color
    int lchild;
    int rchild;
    int root;
    char identifier[16];
    unsigned int flags;
    unsigned int creation_time[2];
    unsigned int modification_time[2];
    int secid;
    int size;
    int unused;
};

struct doc_file
{
    char* filename;
    FILE* file;
    size_t size;
    unsigned int sector_size;
    struct doc_header* header;
    struct secids *sat_secids;
    struct secids *ssat_secids;
    int dirnum;
    struct directory* directories;
    struct directory* root_stream; // root of directory tree
    struct directory* word_document_stream; // WordDocument stream. all text here.
};

struct doc_file*    doc_init(char* filename);
void                doc_clear(struct doc_file* doc);
ssize_t             get_utf8_text(struct doc_file* doc, char* text, size_t max);

#endif
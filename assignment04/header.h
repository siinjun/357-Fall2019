#ifndef _header_tar
#define _header_tar

#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
struct __attribute__ ((__packed__)) header{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
};

uint32_t extract_special_int(char *where, int len);
int insert_special_int(char *where, size_t size, int32_t val);
void extract(int readfile, char *name);
void createnewfile(char buff[512], char namebuf[256],
                    int readfile, int amount);
int octtodec(int n);
#endif

#ifndef _header_tar
#define _header_tar

#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sysmacros.h>
#include<time.h>
#include<pwd.h>
#include<string.h>
#include<dirent.h>
#include<grp.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<utime.h>

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
char *find_name(char *filename);
char *get_name(struct header hd);
int str_2oct(char *octal);
int get_size_of_file(struct header head);
#endif

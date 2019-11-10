#ifndef _header_tar
#define _header_tar

struct __attribute__ ((__packed__)) header{
    char *name;
    char *mode;
    char *uid;
    char *gid;
    char *size;
    char *mtime;
    char *chksum;
    char *typeflag;
    char *linkname;
    char *magic;
    char *version;
    char *uname;
    char *gname;
    char *devmajor;
    char *devminor;
    char *prefix;
};

#endif

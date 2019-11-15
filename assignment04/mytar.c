#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/sysmacros.h>
#include<unistd.h>
#include<time.h>
#include<pwd.h>
#include<string.h>
#include<dirent.h>
#include<grp.h>
#include<errno.h>
#include "header.h"
#include "ustarformat.c"

#define SE stderr
#define DEBUG false
#define OCTAL_LIMIT 2097151

bool val_head = true;
int P_FLG = 0;
int S_FLG = 0;
int V_FLG = 0;
char *octal_2str(int octal, int length){
    char *str, *padded;
    int zero;

    str = calloc(length, 1);
    sprintf(str, "%o", octal);
    if(!str[length-1]){
        padded = calloc(length,1);
        zero = length - strlen(str) - 1;
        memset(padded, 48, zero);
        padded = strcat(padded, str);
        return padded;
    }
    return str;
}

char *get_mode(struct stat file){
    char *octal;
    int mode;
    mode = file.st_mode & 0777;
    octal = octal_2str(mode, 8);
    return octal;
}

char *get_uid(struct stat file){
    char *octal;
    uid_t uid = file.st_uid;
    if(uid < OCTAL_LIMIT){
        octal = octal_2str(uid, 8);
    }else if (!S_FLG){
        int valid;
        octal = calloc(8,1);
        valid = insert_special_int(octal, 8, uid);
        if(valid){
            perror("Bad uid");
        }
    }else{
        fprintf(SE, "octal value too long. (%o)\n", uid);
        val_head = false;
    }
    return octal;
}

char *get_gid(struct stat file){
    char *octal;
    gid_t gid= file.st_gid;
    if(gid < OCTAL_LIMIT){
        octal = octal_2str(gid, 8);
    }else{
        int valid;
        octal = calloc(8,1);
        valid = insert_special_int(octal, 8, gid);
        if(valid){
            perror("Bad gid");
        }
    }
    return octal;
}

char *get_size(struct stat file){
    char *octal;
    octal = octal_2str(file.st_size, 12);
    return octal;
}

char *get_mtime(struct stat file){
    char *octal;
    octal = octal_2str(file.st_mtime, 12);
    return octal;
}

char *get_chksum(struct header header){
    char *hp;
    char *octal, space = ' ';
    /*initialized w 8 bc chksum assumed to be all spaces*/
    int i,chars = 8 * (int)space;

    hp = (char*)&header;
    for(i=0; i < sizeof(struct header); i++){
        if(hp[i] != '\0'){
            chars += (unsigned char)hp[i];
        }
    }
    octal = octal_2str(chars, 8);
    return octal;
}

char get_typeflag(struct stat file){
    mode_t mode = file.st_mode;
    char type;
    if(S_ISREG(mode))
        type = '0';
    else if(S_ISDIR(mode))
        type = '5';
    else
        type = '2';
    return type;
}

char *get_linkname(char *name){
    /* may be wrong*/
    char *path;
    int valid;
    path = calloc(105, 1);
    valid = readlink(name, path,100);
    if (valid == -1){
        perror("readlink");
        exit(1);
    }
    if(strlen(path) > 100){
        val_head = false;
        return NULL;
    }
    return path;
}

char  *get_magic(){
    char *magic;
    magic = malloc(6);

    magic[0] = 'u';
    magic[1] = 's';
    magic[2] = 't';
    magic[3] = 'a';
    magic[4] = 'r';
    if(S_FLG){
    magic[5] = ' ';
    }else{
        magic[5] = '\0';
    }
    return magic;

}

char *get_version(){
    char *version;
    version = malloc(2);

    version[0] = '0';
    version[1] = '0';
    return version;
}

char *get_uname(struct stat file){
    struct passwd *pw;
    char *uname;

    pw = malloc(sizeof(struct passwd));
    pw = getpwuid(file.st_uid);
    uname = malloc(32);
    memcpy(uname, pw->pw_name, 32);
    return uname;
}

char *get_gname(struct stat file){
    struct group *gp;
    char *gname;

    gp = malloc(sizeof(struct group));
    gp = getgrgid(file.st_gid);
    gname = malloc(32);
    memcpy(gname, gp->gr_name, 32);
    return gname;
}

char *get_devmajor(struct stat file){
    unsigned int maj;
    char *octal=NULL;

    dev_t dev = file.st_rdev;
    octal = calloc(1,8);
    maj = major(dev);
    if(maj)
        octal = octal_2str(maj, 8);
    return octal;
}

char *get_devminor(struct stat file){
    unsigned int min;
    char *octal=NULL;

    dev_t dev = file.st_rdev;
    octal = calloc(1,8);
    min = minor(dev);
    if(min)
        octal = octal_2str(min, 8);
    return octal;
}

void get_name_pre(struct stat file, struct header *head,
                const char *path, const char *name){

    int i,pref=0, count=0, len = strlen(path) + strlen(name);
    char *full, *tmp;

    if(len > 256){
        val_head = false;
        return;
    }else{
        full = calloc(256, 1);
        full = strcpy(full, path);
        full = strcat(full, name);
        tmp = calloc(256, 1);
        for(i=0; i < len; i++){
            if(full[i] != '/'){
                tmp[count++] = full[i];
            }
            /*partitiion time*/
            else{
                if(count <= 100)
                    tmp[count++] = '/';/*keep adding to name*/
                else if(count <= 155){
                    if(count + pref > 155){
                        val_head = false;
                        free(full);
                        free(tmp);
                        return;
                    }
                    strncat(head->prefix, tmp,count);
                    pref = count;
                    count = 0;
                    memset(tmp,0,len);
                }
                else{
                    val_head = false;
                    free(full);
                    free(tmp);
                    return;
                }
            }
        }
        if(S_ISDIR(file.st_mode) && val_head){
            strcat(tmp, "/");
            count++;
        }
        if(count<=100){
            strncpy(head->name, tmp,count);
        }
        else if((pref+count-1)<=155 && S_ISDIR(file.st_mode)){
            strncpy(head->prefix, tmp,count-1);
        }
        else{
            val_head = false;
        }
        free(full);
        free(tmp);
    }
}
struct header create_header(struct stat file, char *name, char *path){
    struct header head;
    char *tmp;
    int i = 0;
    /*initialize struct to 0*/
    memset(&head, 0, sizeof(struct header));
    /*initialize that file may be used for valid header*/
    val_head = true;
    for(;i<1;i++){
        get_name_pre(file, &head, path, name);
        if(val_head == false){
            fprintf(SE, "%s%s: unable to construct header.",path,name);
            fprintf(SE, " (Name too long?)");
            fprintf(SE," Skipping.\n");
            fflush(stderr);
            return head;
        }
        strncpy(head.mode, get_mode(file),8);

        tmp = get_uid(file);
        if(val_head){
            memcpy(head.uid, tmp, 8);
        } else{
            fprintf(SE,"%s%s: Unable to create ",path, name);
            fprintf(SE,"conforming header.  Skipping.\n");
            fflush(stderr);
            return head;
        }

        tmp = get_gid(file);
        if(val_head){
            memcpy(head.gid, tmp, 8);
        }else{
            fprintf(SE,"%s%s: Unable to create ",path, name);
            fprintf(SE,"conforming header.  Skipping.\n");
            fflush(stderr);
            return head;
        }

        if(S_ISREG(file.st_mode)){
            strncpy(head.size, get_size(file),12);
        }else{
            strcpy(head.size, "00000000000");
        }
        strcpy(head.mtime, get_mtime(file));
        if(S_ISLNK(file.st_mode)){
            strcpy(head.linkname, get_linkname(name));
        }
        strncpy(head.magic, get_magic(), 6);
        strncpy(head.version, get_version(),2);
        strncpy(head.uname, get_uname(file),32);
        strncpy(head.gname, get_gname(file),32);
        head.typeflag = get_typeflag(file);
        strncpy(head.devmajor, get_devmajor(file),8);
        strncpy(head.devminor, get_devminor(file),8);
        strncpy(head.chksum, get_chksum(head),8);
    }
    if(DEBUG){
        printf("path/name: %s\n", head.name);
        printf("mode: %s\n", head.mode);
        printf("uid: %s\n", head.uid);
        printf("gid: %s\n", head.gid);
        printf("size: %s\n", head.size);
        printf("mtime: %s\n", head.mtime);
        printf("chcksum %s\n", head.chksum);
        printf("typeflag: %c\n", head.typeflag);
        printf("linkename: %s\n", head.linkname);
        printf("magic: %s\n", head.magic);
        printf("version: %s\n", head.version);
        printf("uname: %s\n", head.uname);
        printf("gname: %s\n", head.gname);
        printf("devmajor: %s\n", head.devmajor);
        printf("devminor: %s\n", head.devminor);

    }
    return head;
}


void write_to_file(struct header head, struct stat file,
                    char *name, int fd, int bd){
    /* write to argv[2] but for now use test.tar*/
    char *cont;
    size_t size = file.st_size;
    int od, leftover;
    if(V_FLG){
        printf("%s%c%s\n", head.prefix,(head.prefix[0])?'/':'\0', head.name);
        fflush(stdout);
    }
    write(fd, &head, sizeof(head));
    lseek(fd, 12, SEEK_CUR);
    /*if reg file*/
    if(!S_ISDIR(file.st_mode) && !S_ISLNK(file.st_mode)){
        cont = malloc(size);
        /*get contents of file*/
        od = open(name, O_RDONLY);
        read(od, cont, size);
        /*change to dir where file exists*/
        fchdir(bd);
        /*write contents of file to new file*/
        write(fd, cont, size);

        leftover = size % 512;
        if(leftover != 0){
            leftover = 512 - leftover;
        }
        lseek(fd, leftover, SEEK_CUR);
        free(cont);
    }
}

void end_padding(int fd){
    /*pad ending with two blocks of 512 Bytes*/
    char *zero;
    zero = calloc(1024, 1);

    write(fd, zero, 1024);
    free(zero);
}
char *find_name(char *filename){
    int i, len = strlen(filename);
    char *name;

    name = calloc(1, len+1);
    for(i = len-1; i >= 0; i--){
        if(filename[i] != '/'){
            name[i] = filename[i];
        }
        else{
            break;
        }
    }
    name[len] = '\0';
    if(i!=-1){
        name += ++i;
    }
    return name;
}

char *find_path(char *filename){
    int i, len = strlen(filename);
    char *path;
    path = calloc(256,1);

    for(i = len -1; i >= 0; i--){
        if(filename[i] == '/'){
            break;
        }

    }
    if(i != -1){
        memcpy(path, filename, ++i);
    }
    return path;
}

void traverse_dir(char *dir_name, char *path, int fd, int begin_dir){

    struct dirent *de;
    struct stat curr;
    struct header head;
    DIR *dir;
    char *name, *new_path, *cwd;
    int valid;

    if(DEBUG){
        cwd = calloc(356, 1);
        getcwd(cwd, 356);
        printf("cwd: %s\n", cwd);
    }
    de = malloc(sizeof(struct dirent));
    if(strlen(path)){
        valid = chdir(dir_name);
        if(valid == -1){
            perror("chdir");
            exit(1);
        }
    }
    dir = opendir(".");
    if(errno){
        perror(dir_name);
        exit(1);
    }
    while((de = readdir(dir)) != NULL){
        name = de->d_name;
        if(strcmp(name, ".") && strcmp(name, "..")){
            valid = lstat(name, &curr);
            if(valid < 0){
                perror("lstat");
                exit(1);
            }
            head = create_header(curr, name, path);
            /*check if valid header*/
            if(val_head){
                int cur_dir;
                cur_dir = open(".", O_RDONLY);
                write_to_file(head, curr, name, fd, begin_dir);
                fchdir(cur_dir);
                close(cur_dir);
            }
            /*dont recurse if "." or ".."*/
            if((S_ISDIR(curr.st_mode))){
                dir_name = name;
                new_path = calloc(256, 1);
                new_path = strcpy(new_path, path);
                new_path = strcat(new_path, name);
                new_path = strcat(new_path,"/");
                traverse_dir(dir_name, new_path, fd, begin_dir);
                free(new_path);
                /*go back to where u came from*/
                chdir("..");
            }
        }
    }
    free(de);
    rewinddir(dir);
    closedir(dir);
}

void read_file(char *filename, int fd, int begin_dir){
    struct stat curr;
    struct header head;
    int valid, cur_dir, end = strlen(filename)-1;
    char *name, *path;

    if(filename[end] == '/'){
        filename[end] = '\0';
    }
    valid = lstat(filename, &curr);
    if(valid == -1){
        perror(filename);
        return;
    }
    name = find_name(filename);
    path = find_path(filename);
    head = create_header(curr, name, path);
    if(val_head){
        cur_dir = open(".", O_RDONLY);
        write_to_file(head, curr, filename, fd, begin_dir);
        fchdir(cur_dir);
        close(cur_dir);
    }
    if(S_ISDIR(curr.st_mode) && (strcmp(name, ".") || strcmp(name, ".."))){
        memcpy(path, filename, strlen(filename));
        path = strcat(path, "/");
        traverse_dir(filename, path, fd, begin_dir);
    }
    free(path);
}

void creation(int v_flg, int s_flg, int argc, char *argv[]){

    int begin_dir,i,fd;

    if(v_flg){
        V_FLG = 1;
    }
    if(s_flg){
        S_FLG = 1;
    }
    begin_dir = open(".", O_RDONLY);
    fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1){
        fprintf(stderr, "%s: Bad file descriptor.\n", argv[2]);
        exit(1);
    }
    for(i = 3; i < argc; i++){
        read_file(argv[i], fd, begin_dir);
        fchdir(begin_dir);
    }
    end_padding(fd);

}
/*
int main(int argc, char *argv[]){
    int i,fd, begin_dir;

    if(argc < 3){
        printf("Usage: mytar [ctvS]f tarfile [ path [ ... ]  ]\n");
        exit(1);
    }
    else{
        begin_dir = open(".", O_RDONLY);
        fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        for(i =0; i<strlen(argv[1]); i++){
            if(argv[1][i] == 'S'){
                S_FLG = 1;
            }
            if(argv[1][i] == 'v'){
                V_FLG = 1;
            }
        }
        for(i = 3; i < argc; i++){
            read_file(argv[i], fd, begin_dir);
            fchdir(begin_dir);
        }
        end_padding(fd);
    }
    return 0;
}*/

#include "header.h"

void writefile(char *filename, int fd, int size, int perms){
    char *cont;
    int newfile, leftover;

    cont = malloc(size);
    read(fd, cont, size);
    newfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, perms);
    write(newfile, cont, size);
    if(size != 0){
        leftover = size % 512;
        leftover = 512 - leftover;
        lseek(fd, leftover, SEEK_CUR);
    }
}

void change_time_ownership(char *filename, int time, int uid, int gid){
    struct stat new;
    struct utimbuf new_time;
    lstat(filename, &new);
    new_time.actime = new.st_atime;
    new_time.modtime = time;
    utime(filename, &new_time);
    lchown(filename, uid, gid);


}

void change_dir(char *name){
    int val,i,count=0, len = strlen(name);
    char *tmp;
    DIR *dir;

    tmp = calloc(257, 1);
    for(i=0; i < len; i++){
        if(name[i]!= '/'){
            tmp[count++] = name[i];
        }
        else{
            dir = opendir(tmp);
            if(dir){
                chdir(tmp);
            }
            else if(ENOENT == errno){
                mkdir(tmp, 0777);
                val = chdir(tmp);
                if(val == -1){
                    fprintf(stderr, "Failed to make dir (%s).\n", tmp);
                }
            }
            else{
                fprintf(stderr, "Bad path, bailing.\n");    
                exit(1);
            }
            memset(tmp, 0 , count);
            count =  0;
        }
    }
    free(tmp);
}

/*extract all*/
void myextraction(int fd, int v_flg){
    struct header hd;
    bool done = false;
    int size, cur_dir, perms, empty=0, time, uid, gid;
    char *fullpath, *onlyname;

    cur_dir = open(".", O_RDONLY);

    while(!done){
        /*always go to dir w tar file*/
        fchdir(cur_dir);

        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);

        if(hd.name[0] || hd.prefix[0]){
            fullpath = get_name(hd);
            size = get_size_of_file(hd);
            change_dir(fullpath);
            perms = str_2oct(hd.mode);
            time = str_2oct(hd.mtime);
            onlyname = find_name(fullpath);
            if(hd.uid[0] != 0){
                uid = extract_special_int(hd.uid, 8);
            } else{
                uid = str_2oct(hd.uid);
            }
            if(hd.gid[0] != 0){
                gid = extract_special_int(hd.gid, 8);
            } else{
                gid = str_2oct(hd.gid);
            }
            if(v_flg){
                printf("%s\n", fullpath);
            }
            if(hd.typeflag != '2'){
                writefile(onlyname,fd, size, perms);
            } else{
                symlink(onlyname, hd.linkname);
            }
            change_time_ownership(onlyname, time, uid, gid);
        }
        else{
            if(++empty == 2)
                done = true;
        }

    }

}

void traverse_sub(int fd, int v_flg, char *filename){
    struct header hd;
    bool done = false, valid = false;
    int size, cur_dir, perms, time, n = strlen(filename), uid, gid;
    char *fullpath, *onlyname;

    cur_dir = open(".", O_RDONLY);

    while(!done){
        valid = false;
        /*always go to dir w tar file*/
        fchdir(cur_dir);

        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);

        if(hd.name[0] || hd.prefix[0]){
            fullpath = get_name(hd);
            if(strncmp(filename, fullpath, n)==0){
                valid = true;
            }
            if(valid){
                size = get_size_of_file(hd);
                change_dir(fullpath);
                perms = str_2oct(hd.mode);
                time = str_2oct(hd.mtime);
                onlyname = find_name(fullpath);
                if(hd.uid[0] != 0){
                    uid = extract_special_int(hd.uid, 8);
                } else{
                    uid = str_2oct(hd.uid);
                }
                if(hd.gid[0] != 0){
                    gid = extract_special_int(hd.gid, 8);
                } else{
                    gid = str_2oct(hd.gid);
                }
                if(v_flg){
                    printf("%s\n", fullpath);
                }
                if(hd.typeflag != '2'){
                    writefile(onlyname,fd, size, perms);
                } else{
                    symlink(onlyname, hd.linkname);
                }
                change_time_ownership(onlyname, time, uid, gid);
            }
            else{
                done = true;
            }
        }
        else{
            done = true;
        }

    }

}
void myextraction_named(int fd,int argc, char *argv[], int v_flg){
    struct header hd;
    bool done = false, valid = false, subdir = false;
    int i, size, cur_dir, perms, empty=0, time, uid, gid;
    char *fullpath, *onlyname;

    cur_dir = open(".", O_RDONLY);

    while(!done){
        valid = false;
        /*always go to dir w tar file*/
        fchdir(cur_dir);
        if(subdir){
            lseek(fd, -512, SEEK_CUR);
        }
        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);

        if(hd.name[0] || hd.prefix[0]){
            fullpath = get_name(hd);
            for(i=3; i < argc; i++){
                if(strcmp(argv[i], fullpath)==0){
                    valid = true;
                    break;
                }
            }
            if(valid){
                size = get_size_of_file(hd);
                change_dir(fullpath);
                perms = str_2oct(hd.mode);
                time = str_2oct(hd.mtime);
                onlyname = find_name(fullpath);
                if(hd.uid[0] != 0){
                    uid = extract_special_int(hd.uid, 8);
                } else{
                    uid = str_2oct(hd.uid);
                }
                if(hd.gid[0] != 0){
                    gid = extract_special_int(hd.gid, 8);
                } else{
                    gid = str_2oct(hd.gid);
                }
                if(v_flg){
                    printf("%s\n", fullpath);
                }
                if(hd.typeflag != '2'){
                    writefile(onlyname,fd, size, perms);
                } else{
                    symlink(onlyname, hd.linkname);
                }
                change_time_ownership(onlyname, time, uid, gid);
                if(hd.typeflag == '5'){
                    traverse_sub(fd, v_flg, fullpath);
                    subdir = true;
                }
            }
        }
        else{
            if(++empty == 2)
                done = true;
        }

    }

}


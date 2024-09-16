#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char* pattern){
    int fd;
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", path);
        return;
    }
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
        return;
    }
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        strcpy(p, de.name);
        if(!strcmp(de.name, ".") || !strcmp(de.name, "..")){
            continue;
        }
        if(stat(buf, &st) < 0){
            continue;
        }
        if(!strcmp(de.name, pattern)){
            fprintf(1, "%s\n", buf);
        }
        if(st.type == T_DIR){
            find(buf, pattern);
        }
    }

}

int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(2, "find: argument number error\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    return 0;   
}
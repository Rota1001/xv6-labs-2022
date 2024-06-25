#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

int main(int argc, char* argv[]){
    if(argc != 1){
        fprintf(2, "error");
        exit(1);
    }
    int p[2];
    pipe(p);
    if(fork() == 0){
        char buf[1];
        read(p[0], buf, 1);
        close(p[0]);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], buf, 1);
        close(p[1]);
    }else{
        char buf[1];
        buf[0] = 'a';
        write(p[1], buf, 1);
        close(p[1]);
        read(p[0], buf, 1);
        close(p[0]);
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}
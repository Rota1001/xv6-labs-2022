#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void next_stage(int* pl){
    int prime;
    close(pl[1]);
    int stat = read(pl[0], &prime, sizeof(int));
    if(stat == 0){
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);
    int p[2];
    pipe(p);
    if(fork() == 0){
        close(pl[0]);
        next_stage(p);
    }else{
        close(p[0]);
        int num;
        while(read(pl[0], &num, sizeof(int)) != 0){
            if(num % prime != 0){
                write(p[1], &num, sizeof(int));
            }
        }
        close(pl[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
    
}

int main(){
    int p[2];
    pipe(p);
    if(fork() == 0){
        next_stage(p);
    }else{
        for(int i = 2; i <= 35; i++){
            close(p[0]);
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    // wait(0);
    exit(0);
}
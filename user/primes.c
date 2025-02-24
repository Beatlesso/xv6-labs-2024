#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int fa_fd[2];
int son_fd[2];
void primes(int) __attribute__((noreturn));
void primes(int num) 
{
    if(pipe(son_fd) == -1) exit(1);
    int x;
    int pid = fork();
    if(pid > 0)
    {
        while(read(fa_fd[0], &x, sizeof(x)) != 0)
        {
            if(x % num)
            {
                if(write(son_fd[1], &x, sizeof(x)) == -1) 
                {
                    fprintf(2, "write error\n");
                    exit(1);
                }
            }
        }
        close(fa_fd[0]);
        close(son_fd[1]);    
    }
    else if(pid == 0)
    {
        fa_fd[0] = son_fd[0];
        fa_fd[1] = son_fd[1];
        if(read(fa_fd[0], &x, sizeof(x)) != 0) 
        {
            fprintf(2, "prime %d\n", x);
            primes(x);
        }
    }
    else
    {
        fprintf(2, "fork error\n");
        exit(1);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    if(pipe(fa_fd) == -1) exit(1);
    int pid = fork();
    if(pid > 0)
    {
        for(int i = 2 ; i <= 280 ; ++ i) 
            if(write(fa_fd[1], &i, sizeof(i)) == -1) 
            {
                fprintf(2, "write error\n");
                exit(1);
            }
        close(fa_fd[1]);
        int status;
        wait(&status);
    }
    else if(pid == 0) 
    {
        int x;
        if(read(fa_fd[0], &x, sizeof(x)) != 0) 
        {
            fprintf(2, "prime %d\n", x);
            primes(x);
        }          
    }
    else 
    {
        fprintf(2, "fork error\n");
        exit(1);
    }
    exit(0);
}

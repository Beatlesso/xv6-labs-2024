#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter_num(int) __attribute__((noreturn));
void filter_num(int r_fd)
{
    int prime;
    if(read(r_fd, &prime, sizeof(int)) == 0) 
        exit(0);
    fprintf(2, "prime %d\n", prime);    
    int pfd[2];
    if(pipe(pfd) < 0) exit(1);
    if(!fork())
    {
        int num;
        while(read(r_fd, &num, sizeof(int)) > 0)
        {
            if(num % prime)
                write(pfd[1], &num, sizeof(int));
        }
        close(pfd[1]);
        close(r_fd);
        exit(0);
    }
    else
    {
        close(pfd[1]);
        close(r_fd);
        filter_num(pfd[0]);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    int pfd[2];
    if(pipe(pfd) < 0) exit(1);
    int r_fd = pfd[0], w_fd = pfd[1];
    if(fork() > 0) 
    {
        for(int i = 2 ; i <= 280 ; i ++)
            write(w_fd, &i, sizeof(int));
        close(w_fd);
        while(wait(0) > 0);
        exit(0);
    } 
    else 
    {
        close(w_fd);
        filter_num(r_fd);
    }
}

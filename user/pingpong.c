#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[32];
    int p[2];
    if(pipe(p) == -1) exit(1);

    int pid = fork();
    if(pid > 0) 
    {
        write(p[1], buf, 1);
        int id = getpid();
        int status = 0;
        wait(&status);
        read(p[0], buf, 1);
        fprintf(2, "%d: received pong\n", id);
    } 
    else if(pid == 0) 
    {
        read(p[0], buf, 1);
        int id = getpid();
        fprintf(2, "%d: received ping\n", id);
        write(p[1], buf, 1);
    } else 
    {
        fprintf(2, "fork error\n");
        exit(1);
    }

    exit(0);
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"


int main(int argc, char *argv[])
{
    char* params[MAXARG];
    int idx = 0, p = 0;
    for(int i = 1 ; i < argc ; i ++)
    {
        params[idx] = malloc(sizeof(char) * MAXARG);
        for(int j = 0 ; j < strlen(argv[i]) ; j ++)
            params[idx][p ++] = argv[i][j];
        params[p] = '\0';
        idx ++, p = 0;
    }
    // fprintf(2, "%d\n", idx);
    // for (int i = 0; i < idx; i++)
    //     fprintf(2, "%s\n", params[i]);
    int pre_idx = idx;
    while(1) 
    {
        char c;
        if(read(0, &c, 1) <= 0) break;
        if(c == '\n') 
        {
            params[idx][p] = '\0';
            idx = pre_idx;
            p = 0;
            if(!fork()) exec(argv[1], params);
            else wait(0);
        }
        else if(c == ' ')
        {
            if(p > 0)
            {
                params[idx][p] = '\0';
                idx ++, p = 0;
            }
        }
        else 
        {
            if(!p) params[idx] = malloc(sizeof(char) * MAXARG);
            params[idx][p ++] = c;
        }
    }
    if(idx > pre_idx)
        exec(argv[1], params);
    // exec(argv[1], params);
    // fprintf(2, "%d\n", idx);
    // for (int i = 0; i < idx; i++)
    //     fprintf(2, "%s\n", params[i]);
    
    exit(0);
}
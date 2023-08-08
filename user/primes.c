#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
 
int main(int argc, char *argv[])
{
    int prev[2];
    pipe(prev);
    int ret = fork();
    if (ret < 0)
    {
        fprintf(2, "error in fork\n");
    }
    else if (ret == 0)
    {
        int base = 0;
        while (1)
        {
            close(prev[1]); //关闭写端
            if (read(prev[0], &base, sizeof(base)) == 0)
            {
                exit(0);
            }
            printf("prime %d\n", base);
            int next[2];
            pipe(next);
            int r = fork();
            // 父进程从读端筛选质数
            if (r > 0)
            {
                int cur = 0;
                close(next[0]);
                for (; read(prev[0], &cur, sizeof(cur)) != 0;)
                {
                    if (cur % base != 0)
                    {
                        write(next[1], &cur, sizeof(cur));
                    }
                }
                close(prev[0]);
                close(next[1]);
                wait(0);
                exit(0);
            }
            close(prev[0]);
            prev[0] = next[0];
            prev[1] = next[1];
        }
    }
    else
    {
        close(prev[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(prev[1], &i, sizeof(int));
        }
        close(prev[1]);
        wait(0);
    }
    exit(0);
}
#include "shared.h"

int main(int argc, char *argv[])
{
    int n, l, i = 0, sh_fd, co_fd, *count, *busy, bu_fd, sb_fd,max;
    FILE *input;
    sem_t *sem;
    sem = sem_open("/sem1", O_CREAT | O_RDWR | O_EXCL,0775, 1);
    shm_unlink(Schedule);
    shm_unlink(MatchCount);
    shm_unlink(Busy);
    shm_unlink(Scoreboard);
    input = fopen(argv[1], "r");
    fscanf(input, "%d", &n);
    int size = n * (n - 1);
    sh_fd = shm_open(Schedule, O_CREAT | O_RDWR | O_EXCL, 0775);
    co_fd = shm_open(MatchCount, O_CREAT | O_RDWR | O_EXCL, 0775);
    bu_fd = shm_open(Busy, O_CREAT | O_RDWR | O_EXCL, 0775);
    sb_fd = shm_open(Scoreboard, O_CREAT | O_RDWR | O_EXCL, 0775);
    ftruncate(sh_fd, sizeof(struct schedule1) * size);
    ftruncate(co_fd, sizeof(int));
    ftruncate(bu_fd, sizeof(int) * n);
    ftruncate(sb_fd, sizeof(struct scoreboard1) * n);
    struct schedule1 *sp;
    struct scoreboard1 *sb,temp;
    sp = (struct schedule1 *)mmap(NULL, sizeof(struct schedule1) * size, PROT_READ | PROT_WRITE, MAP_SHARED, sh_fd, 0);
    count = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, co_fd, 0);
    busy = (int *)mmap(NULL, sizeof(int) * n, PROT_READ | PROT_WRITE, MAP_SHARED, bu_fd, 0);
    sb = (struct scoreboard1 *)mmap(NULL, sizeof(struct scoreboard1) * n, PROT_READ | PROT_WRITE, MAP_SHARED, sb_fd, 0);
    *count = 0;
    while (1)
    {
        int t1, t2;
        l = fscanf(input, "%d %d", &t1, &t2);
        if (l == -1)
            break;
        sp[i].home = t1;
        sp[i].against = t2;
        i++;
    }
    for (int j = 0; j < size; j++)
    {
        sp[j].done = 0;
    }
    for (int j = 0; j < n; j++)
    {
        busy[j] = 0;
        sb[j].team_id  = j+1;
        sb[j].win = 0;
        sb[j].draw = 0;
        sb[j].lose = 0;
        sb[j].goal = 0;
        sb[j].point = 0;
    }
    for(int j=0;j<n;j++)
    {
        if(!fork())
        {
            char str[256];
            sprintf(str, "%d",j+1);
            char str1[256];
            sprintf(str1, "%d", n);
            char *args[] = {"./manager.out",str,str1,NULL};
            execv("./manager.out", args);
        }
    }
    while(wait(NULL)!=-1);
    for(int j=0;j<n;j++)
    {
        max=j;
        for(int k=j+1;k<n;k++)
        {
            if(sb[k].point>sb[max].point)
                max=k;
            else if (sb[k].point==sb[max].point)
            {
                if(sb[k].goal>sb[max].goal)
                    max = k;
                else if(sb[k].goal == sb[max].goal)
                {
                    if(sb[k].team_id<sb[max].team_id)
                        max = k;
                }
            }
        }
        temp = sb[j];
        sb[j] = sb[max];
        sb[max] = temp;
    }
    printf("\n");
    printf("Team\tWin\tDraw\tLose\tGS\tPoints\n");
    for (int j = 0; j < n; j++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",sb[j].team_id,sb[j].win,sb[j].draw,sb[j].lose,sb[j].goal,sb[j].point);
    }
    return 0;
}
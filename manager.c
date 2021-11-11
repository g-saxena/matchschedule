#include "shared.h"
int main(int argc, char* argv[])
{
    int n,self,size,*count,co_fd,sh_fd,bu_fd,*busy,i,sb_fd;
    sscanf(argv[1],"%d",&self);
    sscanf(argv[2],"%d",&n);
    sem_t *sem;
    sem = sem_open("/sem1",O_RDWR);
    size = n*(n-1);
    sh_fd = shm_open(Schedule, O_RDWR, 0775);
    co_fd = shm_open(MatchCount,O_RDWR , 0775);
    bu_fd = shm_open(Busy,O_RDWR, 0775);
    sb_fd = shm_open(Scoreboard, O_RDWR , 0775);
    struct schedule1 *sp;
    struct scoreboard1 *sb;
    sp = (struct schedule1 *)mmap(NULL, sizeof(struct schedule1 )*size,PROT_READ | PROT_WRITE,MAP_SHARED,sh_fd, 0);
    count = (int *)mmap(NULL, sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,co_fd, 0);
    busy = (int *)mmap(NULL, sizeof(int)*n,PROT_READ | PROT_WRITE,MAP_SHARED,bu_fd, 0); 
    sb = (struct scoreboard1 *)mmap(NULL, sizeof(struct scoreboard1)*n,PROT_READ | PROT_WRITE,MAP_SHARED,sb_fd, 0);
    srand(getpid());
    while(*count != size)
    {
        for(i=0;i<size;i++)
        {
            if(sp[i].home==self && sp[i].done==0)
            {
                int flag=0;
                sem_wait(sem);
                if(busy[sp[i].home-1]==0 && busy[sp[i].against-1]==0)
                {
                    busy[sp[i].home-1]=1;
                    busy[sp[i].against-1]=1;
                    flag=1;
                }
                sem_post(sem);
                if(flag==1)
                {
                    fflush(stdout);
                    printf("Starting match: Team %d vs Team %d\n",sp[i].home,sp[i].against);
                    int t1g = rand()%6;
                    int t2g = rand()%6;
                    sleep(3);
                    if(t1g > t2g)
                    {
                        sb[sp[i].home-1].win ++;
                        sb[sp[i].home-1].point +=3;
                        sb[sp[i].home-1].goal +=t1g;
                        sb[sp[i].against-1].lose ++;
                        sb[sp[i].against-1].goal +=t2g;

                    }
                    else if(t1g == t2g)
                    {
                        sb[sp[i].home-1].draw ++;
                        sb[sp[i].home-1].goal +=t1g;
                        sb[sp[i].home-1].point +=1;
                        sb[sp[i].against-1].draw ++;
                        sb[sp[i].against-1].goal +=t2g;
                        sb[sp[i].against-1].point +=1;
                    }
                    else
                    {
                        sb[sp[i].against-1].win ++;
                        sb[sp[i].against-1].point +=3;
                        sb[sp[i].against-1].goal +=t2g;
                        sb[sp[i].home-1].lose ++;
                        sb[sp[i].home-1].goal +=t1g;
                    }
                    sem_wait(sem);
                    busy[sp[i].home-1]=0;
                    busy[sp[i].against-1]=0;
                    sp[i].done=1;
                    fflush(stdout);
                    printf("Match ended: Team %d vs Team %d\tResult:%d-%d\n",sp[i].home,sp[i].against,t1g,t2g);
                    *count +=1;
                    sem_post(sem);
                }
            }
        }
    }
    return 0;
}
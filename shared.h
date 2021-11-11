#ifndef SHARED
#define SHARED

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h> 
#include <sys/mman.h>
#include <mqueue.h>
#include <wait.h>
#include <math.h>
struct schedule1
{
    int home;
    int against;
    int done;
};
struct scoreboard1
{
    int team_id;
    int win;
    int draw;
    int lose;
    int goal;
    int point;
};

#define Schedule "/SharedSchedule"
#define MatchCount "/SharedMatchCount"
#define Busy "/SharedBusy"
#define Scoreboard "/SharedScoreboard"
#endif
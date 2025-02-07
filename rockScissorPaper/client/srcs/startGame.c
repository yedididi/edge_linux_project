#include "../incs/define.h"

void startGame(int sfd, char *dev_name)
{
    int whichClient = 0;
    pthread_t sendingThreadID;
    t_info *info; //dont forget to free at all exit
    //t_map *map; //dont forget to free at all exit

    char buf[MAX_BUF];
    int readRet = read(sfd, buf, MAX_BUF);
    buf[readRet] = '\0';
    if (strncmp(buf, "C1\0", 3) == 0)
        whichClient = C1;
    else if (strncmp(buf, "C2\0", 3) == 0)
        whichClient = C2;

    info = fillInfo(sfd, dev_name, whichClient);
    
    draw_map(info->map);
    printf("[%d] creating thread\n", getpid());
    int ret = pthread_create(&sendingThreadID, NULL, &sendingThread, info);
    if(ret != 0) 
    {
        printf("[%d] error: %d (%d)\n", getpid(), ret, __LINE__);
        exit(1);
    }

    mainThread(info, sfd);
}

t_info *fillInfo(int sfd, char *dev_name, int whichClient)
{
    t_info *info;

    info = (t_info *)malloc(sizeof(t_info));
    if (info == NULL)
        printf("malloc error\n");
    info->serverFD = sfd;
    info->dev_name = dev_name;
    info->turn = C1;
    info->whichClient = whichClient;
    info->map = map_init();
    return (info);
}

void mainThread(t_info *info, int sfd)
{
    for (;;)
    {
        char buf[MAX_BUF];
        read(sfd, buf, MAX_BUF);

        t_gameInfo *gameInfo = (t_gameInfo *)buf;
        
        printf("rcvd from server. gameStatus:%d, (%d, %d)\n", gameInfo->gameStatus, gameInfo->i, gameInfo->j);
        
        if (gameInfo->gameStatus == GAMEOVER)
        {
            printEndScreen();
            sleep(3);
            exit(0);
        }
        else
        {
            printRock(info, gameInfo->i, gameInfo->j);

            if (info->turn == C1)
                info->turn = C2;
            else if (info->turn == C2)
                info->turn = C1;
        }
    }
}

void printRock(t_info *info, int i, int j)
{
    draw_circle((i * 45 + 220), (j * 45) + 60, 5, COLOR_BLACK, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
}

void printEndScreen()
{

}
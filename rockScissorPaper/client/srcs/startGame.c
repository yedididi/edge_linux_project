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
        whichClient = 1;
    else if (strncmp(buf, "C2\0", 3) == 0)
        whichClient = 2;
    info = fillInfo(sfd, dev_name, whichClient);
    
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
            draw_turn(info->map);
            if (info->turn == C1)
                info->turn = C2;
            else if (info->turn == C2)
                info->turn = C1;
            //printRock(gameInfo->i, gameInfo->j);
        }
    }
}

void printRock(int i, int j)
{
    (void)i;
    (void)j;
}

void printEndScreen()
{

}
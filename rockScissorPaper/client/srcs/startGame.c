#include "../incs/define.h"

void startGame(int sfd, char *dev_name)
{
    pthread_t sendingThreadID;
    t_info *info; //dont forget to free at all exit
    //t_map *map; //dont forget to free at all exit

    info = fillInfo(sfd, dev_name);
    
    printf("[%d] creating thread\n", getpid());
    int ret = pthread_create(&sendingThreadID, NULL, &sendingThread, info);
    if(ret != 0) 
    {
        printf("[%d] error: %d (%d)\n", getpid(), ret, __LINE__);
        exit(1);
    }

    mainThread(sfd);
}

t_info *fillInfo(int sfd, char *dev_name)
{
    t_info *info;

    info = (t_info *)malloc(sizeof(t_info));
    if (info == NULL)
        printf("malloc error\n");
    info->serverFD = sfd;
    info->dev_name = dev_name;
    info->map = map_init();
    return (info);
}

void mainThread(int sfd)
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
            printRock(gameInfo->i, gameInfo->j);
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
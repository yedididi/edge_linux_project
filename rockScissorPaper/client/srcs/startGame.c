#include "../incs/define.h"

void startGame(int sfd, char *dev_name)
{
    int whichClient = 0;
    pthread_t sendingThreadID;
    t_info *info; //dont forget to free at all exit

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
    int whoWon = 0;

    for (;;)
    {
        char buf[MAX_BUF];
        read(sfd, buf, MAX_BUF);

        t_gameInfo *gameInfo = (t_gameInfo *)buf;
        
        printf("rcvd from server. gameStatus:%d, (%d, %d), ", gameInfo->gameStatus, gameInfo->i, gameInfo->j);
        if (gameInfo->color == COLOR_BLACK)
            printf("BLACK\n");
        else if (gameInfo->color == COLOR_WHITE)
            printf("WHITE\n");
            
        if (gameInfo->gameStatus == PLAYING)
            whoWon = gameInfo->color;
        
        if (gameInfo->gameStatus == GAMEOVER)
        {
            printEndScreen(info, whoWon);
            sleep(3);
            exit(0);
        }
        else
        {
            printRock(info, gameInfo->i, gameInfo->j, gameInfo->color);
            if (info->turn == C1)
                info->turn = C2;
            else if (info->turn == C2)
                info->turn = C1;
        }
    }
}

void printRock(t_info *info, int i, int j, int color)
{
    draw_circle((i * 45 + 220), (j * 45) + 60, 22, color, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
}


void printEndScreen(t_info *info, int whoWon)
{
    draw_rect(0, 0, (info->map->vinfo).xres, (info->map->vinfo).yres, COLOR_BLACK, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);

    draw_rect(315, 195, 150, 95, COLOR_GOLD, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
    draw_text("GAME OVER", 320, 200, 4, COLOR_WHITE, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
    if (whoWon == COLOR_BLACK)
        draw_text("BLACK WIN", 320, 250, 4, COLOR_WHITE, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
    else if (whoWon == COLOR_WHITE)
        draw_text("WHITE WIN", 320, 250, 4, COLOR_WHITE, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
}
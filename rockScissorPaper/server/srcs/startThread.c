#include "../incs/define.h"

//return 0 if success, else return 1
void *startThread(void *info_)
{
    int *ret = 0;
    t_info *info = (t_info *)info_;
    
    printf("thread started\n");

    //send "playStart" if two players connect
    while (1)
    {
        pthread_mutex_lock(&(info->playerNumMuxtex));
        if (*(info->playerNum) == 2)
        {
            write(info->clientfd, "playStart", 9);
            printf("[%d] playStart sent\n", info->clientfd);
            pthread_mutex_unlock(&(info->playerNumMuxtex));

            if (info->whichClient == C1)
                write(info->clientfd, "C1", 2);
            else if (info->whichClient == C2)
                write(info->clientfd, "C2", 2);

            omokStart(info);
            break;
        }
        pthread_mutex_unlock(&(info->playerNumMuxtex));
        usleep(1000);
    }
    printf("thread ending\n");
	pthread_exit(ret);
}  

void omokStart(t_info *info)
{
    printf("sub thread starting\n");
    while (1)
    {
        switch (*(info->gameStatus))
        {
            case (C1_WAITING):
            {
                //printf("game status:C1_WAITING\n");
                if (info->whichClient == C1)
                {
                    printf("game status:C1_WAITING, this client is C1\n");
                    char buf[MAX_BUF];

                    int readRet = read(info->clientfd, buf, MAX_BUF);
                    if (readRet < 0)
                    {
                        printf("read error at C1_WAITING\n");
                        exit(1);
                    }

                    t_gameInfo *gameInfoRCVD = (t_gameInfo *)buf;

                    printf("arrived from C1, (%d, %d)\n", gameInfoRCVD->i, gameInfoRCVD->j);

                    info->gameInfo->gameStatus = gameInfoRCVD->gameStatus;
                    info->gameInfo->i = gameInfoRCVD->i;
                    info->gameInfo->j = gameInfoRCVD->j;
                    if (info->turn == C1)
                    {
                        info->gameInfo->color = COLOR_BLACK;
                        printf("info->gameInfo->color is BLACK\n");
                    }
                    else if (info->turn == C2)
                    {
                        info->gameInfo->color = COLOR_WHITE;
                        printf("info->gameInfo->color is WHITE\n");
                    }
                    *(info->gameStatus) = C1_RCVD;
                }
                break;
            }
            case (C2_WAITING):
            {
                //printf("game status:C2_WAITING\n");
                if (info->whichClient == C2)
                {
                    printf("game status:C2_WAITING, this client is C2\n");
                    char buf[MAX_BUF];

                    int readRet = read(info->clientfd, buf, MAX_BUF);
                    if (readRet < 0)
                    {
                        printf("read error at C2_WAITING\n");
                        exit(1);
                    }

                    t_gameInfo *gameInfoRCVD = (t_gameInfo *)buf;

                    printf("arrived from C2, (%d, %d)\n", gameInfoRCVD->i, gameInfoRCVD->j);

                    info->gameInfo->gameStatus = gameInfoRCVD->gameStatus;
                    info->gameInfo->i = gameInfoRCVD->i;
                    info->gameInfo->j = gameInfoRCVD->j;
                    if (info->turn == C1)
                    {
                        info->gameInfo->color = COLOR_BLACK;
                        printf("\ninfo->gameInfo->color is BLACK\n");
                    }
                    else if (info->turn == C2)
                    {
                        info->gameInfo->color = COLOR_WHITE;
                        printf("\ninfo->gameInfo->color is WHITE\n");
                    }
                    *(info->gameStatus) = C2_RCVD;
                }
                break;
            }
            case (C1_SEND):
            {
                if (info->whichClient == C1)
                {
                    printf("game status:C1_SEND\n");
                    t_gameInfo gameInfo;
                    gameInfo.i = info->gameInfo->i;
                    gameInfo.j = info->gameInfo->j;
                    gameInfo.color = info->gameInfo->color;
                    gameInfo.gameStatus = PLAYING;

                    write(info->clientfd, &gameInfo, sizeof(t_gameInfo));

                    printf("writing to client1\n");

                    *(info->gameStatus) = C1_SENT;
                    usleep(1000);
                }
                break;
            }

            case (C2_SEND):
            {
                if (info->whichClient == C2)
                {
                    printf("game status:C2_SEND\n");
                    t_gameInfo gameInfo;
                    gameInfo.i = info->gameInfo->i;
                    gameInfo.j = info->gameInfo->j;
                    gameInfo.color = info->gameInfo->color;
                    gameInfo.gameStatus = PLAYING;

                    write(info->clientfd, &gameInfo, sizeof(t_gameInfo));

                    printf("writing to client2\n");

                    *(info->gameStatus) = C2_SENT;
                    usleep(1000);
                }
                break;
            }

            case (GAMEOVER):
            {
                printf("game status:GAMEOVER\n");
                t_gameInfo gameInfo;
                gameInfo.i = -100;
                gameInfo.j = -100;
                gameInfo.gameStatus = GAMEOVER;
                gameInfo.color = -1;

                write(info->clientfd, &gameInfo, sizeof(t_gameInfo));

                printf("sending gameover\n");

                *(info->gameStatus) = ENDGAME;
                break;
            }
            
        }
    }
}
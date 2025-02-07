#include "../incs/define.h"

void initInfo(t_info **info)
{
    for (int i = 0; i < MAX_BUF; i++)
    {
        info[i] = 0;
    }
}

bool fillInfo(t_info **info, int *playerNum, int sfd_client, int *gameStatus)
{
    *info = (t_info *)malloc(sizeof(t_info));
    if (*info == NULL)
        printf("malloc error\n");
    
    (*info)->playerNum = playerNum;
    (*info)->clientfd = sfd_client;
    (*info)->gameStatus = gameStatus;
    (*info)->turn = C1;
    if (pthread_mutex_init(&((*info)->playerNumMuxtex), NULL) != 0)
		return (EXIT_FAILURE);

    (*info)->gameInfo = (t_gameInfo *)malloc(sizeof(t_gameInfo));
    if ((*info)->gameInfo == NULL)
        printf("malloc error\n");
    else
        printf("gameInfo malloc ok\n");

    (*info)->gameInfo->i = -1;
    (*info)->gameInfo->j = -1;
    (*info)->gameInfo->gameStatus = PLAYING;
    (*info)->gameInfo->color = COLOR_GREEN;
    return (EXIT_SUCCESS);
}

void freeInfo(t_info **info)
{
    for (int i = 0; i < 2; i++)
    {
        printf("freeing no.%d\n", i);
        free(info[i]->gameInfo);
        free(info[i]);
    }
}

void endThread(t_info **info)
{
    for (int i = 0; info[i]; i++)
        pthread_join((info[info[i]->clientfd])->thread_id, NULL);
}
#include "../incs/define.h"

void initInfo(t_info **info)
{
    for (int i = 0; i < MAX_BUF; i++)
    {
        info[i] = 0;
    }
}

bool fillInfo(t_info **info, int *playerNum, int sfd_client)
{
    *info = (t_info *)malloc(sizeof(t_info));
    if (*info == NULL)
        printf("malloc error\n");
    
    (*info)->playerNum = playerNum;
    (*info)->clientfd = sfd_client;
    if (pthread_mutex_init(&((*info)->playerNumMuxtex), NULL) != 0)
		return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

void freeInfo(t_info **info)
{
    for (int i = 0; info[i]; i++)
    {
        free(info[i]);
    }
}
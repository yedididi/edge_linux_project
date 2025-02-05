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
    (void)info;
}
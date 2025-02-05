#include "../incs/define.h"

//return 0 if success, else return 1
void *startThread(void *info_)
{
    int *ret = 0;
    t_info *info = (t_info *)info_;

	//send ok
	write(info->clientfd, "OK", 2);

    //get "ready" from client, else return 1
    char buf[64];
    int retRead = read(info->clientfd, buf, 63);
	buf[retRead] = '\0';
    if (strncmp(buf, "ready\0", 6))
		pthread_exit(NULL);
    
    //send "playStart" if two players connect
    while (1)
    {
        pthread_mutex_lock(&(info->playerNumMuxtex));
        if (*(info->playerNum) == 2)
        {
            write(info->clientfd, "playStart", 9);
            printf("[%d] playStart sent\n", info->clientfd);
            pthread_mutex_unlock(&(info->playerNumMuxtex));
            break ;
        }
        pthread_mutex_unlock(&(info->playerNumMuxtex));
        usleep(1000);
    }
    printf("thread ending\n");
	pthread_exit(ret);
}  
#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdbool.h>

#define SERVER_IP "192.168.0.78"
#define SERVER_PORT 25000
#define LISTEN_BACKLOG 5
#define MAX_BUF 256

typedef struct s_info
{
    int             *playerNum;
    int             clientfd;
    pthread_mutex_t playerNumMuxtex;
    pthread_t       thread_id;
} t_info;

//void *startGame(void *fd_);
int     startGame(int sfd_server, t_info **info, int *playerNum);
int     startTCP(int *sfd_server, struct sockaddr_in *addr_server, int *optval);
bool	fillInfo(t_info **info, int *playerNum, int sfd_client);
void	*startThread(void *info_);
void	freeInfo(t_info **info);
void    omokStart(t_info *info);
void    endThread(t_info **info);


#endif
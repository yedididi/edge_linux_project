#ifndef DEFINE_H
#define DEFINE_H

#define SERVER_IP "192.168.0.78"
#define SERVER_PORT 25000

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

//void *startGame(void *fd_);

typedef struct s_info
{
    int *playerNum;
    int clientfd;
    
} t_info;

#endif
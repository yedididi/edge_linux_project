#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "10.10.141.206"
#define SERVER_PORT 25000
#define MAX_BUF 256

int     connectToServer();
void    startGame(int sfd);

typedef struct s_gameInfo
{
    int i, j;
    int gameStatus;
} t_gameInfo;

#endif
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
#define MAPSIZE 17
#define WINNINGNUM 4 //4가 오목 

typedef enum e_gameStat 
{
    C1_WAITING, 
    C1_RCVD,
    C1_SENT,
    C2_WAITING, 
    C2_RCVD,
    C2_SENT,
    SEND,
    GAMEOVER,
    ENDGAME,
    
    //client status
    PLAYING
} t_gameStat; 

typedef enum e_turn
{
    C1 = 0,
    C2 = 1
} t_turn;

typedef struct s_gameInfo
{
    int i, j;
    int gameStatus;
} t_gameInfo;

typedef struct s_info
{
    int             *playerNum;
    int             clientfd;
    int             *gameStatus;
    int             turn;
    pthread_mutex_t playerNumMuxtex;
    pthread_t       thread_id;
    t_gameInfo      *gameInfo;
} t_info;

//void *startGame(void *fd_);
int     startGame(int sfd_server, t_info **info, int *playerNum, int *gameStatus);
int     startTCP(int *sfd_server, struct sockaddr_in *addr_server, int *optval);
bool    fillInfo(t_info **info, int *playerNum, int sfd_client, int *gameStatus);
int     checkGameOver(int map[MAPSIZE][MAPSIZE], int newI, int newJ);
void	*startThread(void *info_);
void	freeInfo(t_info **info);
void    omokStart(t_info *info);
void    endThread(t_info **info);


#endif
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
#include <linux/fb.h>
#include <sys/mman.h>

#define SERVER_IP "10.10.141.206"
#define SERVER_PORT 25000
#define MAX_BUF 256
#define MAPSIZE 17
#define FBDEV "/dev/fb0"
#define COLOR_RED 0xff0000
#define COLOR_GREEN 0x00ff00
#define COLOR_BLUE 0x0000ff
#define COLOR_BLACK 0x00000f
#define COLOR_WHITE 0xffffff
#define COLOR_BROWN 0xc68a12


typedef struct s_gameInfo
{
    int i, j;
    int gameStatus;
} t_gameInfo;

typedef struct s_info
{
    int serverFD;
    t_gameInfo gameInfo;
} t_info;

typedef struct s_map
{
    int fb_fd;
    int size;
    char *mapNum;
    int color;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

} t_map;

typedef enum e_gameStat 
{
    GAMEOVER,
    PLAYING
} t_gameStat; 

int     connectToServer();
void    startGame(int sfd);
void *sendingThread(void *info_);
void mainThread(int sfd);
t_gameInfo getClick(t_gameInfo gameInfo);
t_info *fillInfo(int sfd);

t_map *map_init();
void draw_map(t_map *map);
void draw_rect(int x, int y, int w, int h, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map);
void draw_circle(int cx, int cy, int r, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map);
void draw_button(t_map *map);

void printRock(int i, int j);
void printEndScreen();

#endif
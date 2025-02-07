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
#define MAPSIZE (10 + 10) 
#define FBDEV "/dev/fb0"

#define COLOR_RED 0xff0000
#define COLOR_GREEN 0x00ff00
#define COLOR_BLUE 0x0000ff
#define COLOR_BLACK 0x00000f
#define COLOR_WHITE 0xffffff
#define COLOR_BROWN 0xc68a12
#define ORIGINAL_MIN 150
#define ORIGINAL_MAX 4000
#define TARGET_WIDTH 800
#define TARGET_HEIGHT 480
#define COLOR_GOLD 0xFFD700

typedef struct s_map
{
    int fb_fd;
    int size;
    char *mapNum;
    int color;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char **clientMap;
} t_map;

typedef struct s_gameInfo
{
    int i, j;
    int gameStatus;
    int color;
} t_gameInfo;

typedef struct s_info
{
    int serverFD;
    char *dev_name;
    int touch_fd;
    int turn;
    int whichClient;
    t_gameInfo gameInfo;
    t_map *map;
} t_info;

typedef enum e_gameStat 
{
    C1_WAITING, 
    C1_RCVD,
    C1_SEND,
    C1_SENT,
    C2_WAITING, 
    C2_RCVD,
    C2_SEND,
    C2_SENT,
    SEND,
    
    GAMEOVER,
    ENDGAME,
    C1_GAMEOVER,
    C2_GAMEOVER,
    
    //client status
    PLAYING
} t_gameStat; 

typedef enum e_turn 
{
    C1,
    C2
} t_turn; 

int     connectToServer();
void startGame(int sfd, char *dev_name);
void *sendingThread(void *info_);
void mainThread(t_info *info, int sfd);
t_gameInfo getClick(t_info *info, int touch_fd);
t_info *fillInfo(int sfd, char *dev_name, int whichClient);

t_map *map_init();
void draw_map(t_map *map);
void draw_rect(int x, int y, int w, int h, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map);
void draw_circle(int cx, int cy, int r, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map);
void draw_target(t_map *map, int x_selected_temp, int y_selected_temp, int x_before_coordinate, int y_before_coordinate);
void add_stone(t_map *map, int x_selected_temp, int y_selected_temp, int x_selected_coordinate_temp, int y_selected_coordinate_temp);
void draw_button(t_map *map);
void draw_turn();
void drawWhoIAm(t_info *info);
void draw_text(const char *text, int x, int y, int scale, unsigned int color,
               struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
               char *map);
void draw_letter(char letter, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map);
void draw_number(int num, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map);

void printRock(t_info *info, int i, int j, int radius, int color);
void printEndScreen(t_info *info, int whoWon);
void printWhoseTurn(t_info *info, int whoseTurn);

#endif
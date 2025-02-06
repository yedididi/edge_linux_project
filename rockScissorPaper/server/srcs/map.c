#include "define.h"

int firstI[4] = {-1, -1, 0, 1};
int firstJ[4] = {0, 1, 1, 1};
int secondI[4] = {1, 1, 0, -1};
int secondJ[4] = {0, -1, -1, -1};

int checkGameOver(int map[MAPSIZE][MAPSIZE], int newI, int newJ)
{
    int currentColor = map[newI][newJ];

    for (int index = 0; index < 4; index++)
    {
        int firstConnectedNum = 0;
        int secondConnectedNum = 0;

        int curI = newI;
        int curJ = newJ;
        while (1)
        {
            curI += firstI[index];
            curJ += firstJ[index];
            if (map[curI][curJ] == currentColor)
                firstConnectedNum++;
            else
                break;
        }
        
        curI = newI;
        curJ = newJ;
        while (1)
        {
            curI += secondI[index];
            curJ += secondJ[index];
            if (map[curI][curJ] == currentColor)
                secondConnectedNum++;
            else
                break;
        }

        if (firstConnectedNum + secondConnectedNum == WINNINGNUM)
            return (GAMEOVER);
    }  
    return (PLAYING);
}
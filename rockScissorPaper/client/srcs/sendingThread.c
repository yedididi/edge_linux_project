#include "../incs/define.h"

void *sendingThread(void *info_)
{
    t_info *info = (t_info *)info_;

    while (1)
    {
        t_gameInfo clickResult;
        clickResult = getClick(clickResult);

        t_gameInfo gameInfo;
        gameInfo.i = clickResult.i;
        gameInfo.j = clickResult.j;
        gameInfo.gameStatus = PLAYING;

        write(info->serverFD, &gameInfo, sizeof(t_gameInfo));
        //usleep()
    }

}

t_gameInfo getClick(t_gameInfo gameInfo)
{

    //...

    return (gameInfo);
}
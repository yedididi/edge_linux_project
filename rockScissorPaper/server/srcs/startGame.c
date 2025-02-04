#include "../incs/startGame.h"

// //return 0 if success, else return 1
// void *startGame(void *fd_)
// {
//     int *ret = 0;
//     int fd = *(int *)fd_;

//     //get "ready" from client, else return 1
//     char buf[64];
//     read(fd, buf, 63);
//     if (strcmp(buf, "ready"))
//     {
//         *ret = 1;
//         return ((void *)ret);
//     }
    
//     //send "playStart" if two players connect
//     while (1)
//     {
//         //mutex lock
//         if (playerNum == 2)
//         {
//             write(fd, "playStart", 9);
//             break ;
//         }
//         //mutex unlock
//         usleep(1000);
//     }

//     return ((void *)ret);
    
// }   
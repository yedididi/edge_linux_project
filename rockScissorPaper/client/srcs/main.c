#include "../incs/define.h"

int main() 
{
    int sfd = connectToServer();

    char buf[MAX_BUF];
    int retRead = read(sfd, buf, 63);
    buf[retRead] = '\0';
    if (strncmp(buf, "playStart", 10) == 0) 
    {
        printf("game start\n");
        startGame(sfd);
    }
    close(sfd);
    return EXIT_SUCCESS;
}

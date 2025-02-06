#include "../incs/define.h"

int main(int argc, char **argv) 
{
    if (argc != 2)
    {
        printf("touchtest: there is no input device\n");
        return EXIT_FAILURE;
    }

    int sfd = connectToServer();

    char buf[MAX_BUF];
    int retRead = read(sfd, buf, 63);
    buf[retRead] = '\0';
    if (strncmp(buf, "playStart", 10) == 0) 
    {
        printf("game start\n");
        startGame(sfd, argv[1]);
    }
    close(sfd);
    return EXIT_SUCCESS;
}

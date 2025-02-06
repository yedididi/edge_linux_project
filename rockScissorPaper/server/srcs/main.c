#include "../incs/define.h"

#define LISTEN_BACKLOG 5
#define MAX_BUF 256

int playerNum = 0;

int main(int argc, char **argv)
{
	int sfd_server = 0;
	struct sockaddr_in addr_server;
	int optval = 1;
	int gameStatus = C1_WAITING;
	int map[MAPSIZE][MAPSIZE] = {0, };

	t_info *info[MAX_BUF];

	if(argc != 1) {
		printf("usage: %s\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("[%d] running %s\n", getpid(), argv[0]);

	if (startTCP(&sfd_server, &addr_server, &optval) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	for(;;) 
	{
		if (startGame(sfd_server, info, &playerNum, &gameStatus) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}

	endThread(info);
	while (1)
	{
		//main thread
		switch (gameStatus)
		{
			case (C1_RCVD):
			{
				int newI = info[0]->gameInfo->i;
				int newJ = info[0]->gameInfo->j;

				map[newI][newJ] = 1;
				info[1]->gameInfo->i = newI;
				info[1]->gameInfo->j = newJ;
				gameStatus = SEND;
				break;
			}
			case (C1_SENT):
			{
				int newI = info[0]->gameInfo->i;
				int newJ = info[0]->gameInfo->j;

				if (checkGameOver(map, newI, newJ) == GAMEOVER)
					gameStatus = GAMEOVER;
				else
				{
					info[0]->turn = C2;
					info[1]->turn = C2;
					gameStatus = C2_WAITING;
				}
				break;
			}

			case (C2_RCVD):
			{
				int newI = info[1]->gameInfo->i;
				int newJ = info[1]->gameInfo->j;

				map[newI][newJ] = 0;
				info[0]->gameInfo->i = newI;
				info[0]->gameInfo->j = newJ;
				gameStatus = SEND;
				break;
			}
			case (C2_SENT):
			{
				int newI = info[1]->gameInfo->i;
				int newJ = info[1]->gameInfo->j;

				if (checkGameOver(map, newI, newJ) == GAMEOVER)
					gameStatus = GAMEOVER;
				else
				{
					info[0]->turn = C1;
					info[1]->turn = C1;
					gameStatus = C1_WAITING;
				}
				break;
			}
			case (ENDGAME):
			{
				freeInfo(info);
				close(sfd_server);

				printf("[%d] terminated\n", getpid());

				return EXIT_SUCCESS;
				break;
			}
		}
	}

	freeInfo(info);
	close(sfd_server);

	printf("[%d] terminated\n", getpid());

	return EXIT_SUCCESS;
}


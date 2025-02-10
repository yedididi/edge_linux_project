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
	int sfd_clients[2] = {0, };

	t_info *info[MAX_BUF];

	if(argc != 1) {
		printf("usage: %s\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("[%d] running %s\n", getpid(), argv[0]);

	if (startTCP(&sfd_server, &addr_server, &optval) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	for (int i = 0; i < 2; i++)
	{
		if (startGame(sfd_server, info, &playerNum, &gameStatus, &(sfd_clients[i])) == EXIT_FAILURE)
			return (EXIT_FAILURE);

	}

	printf("starting main thread\n");
	while (1)
	{
		//main thread
		switch (gameStatus)
		{
			case (C1_RCVD):
			{
				printf("current main state: C1_RCVD\n");
				int newI = info[sfd_clients[0]]->gameInfo->i;
				int newJ = info[sfd_clients[0]]->gameInfo->j;
				int newColor = info[sfd_clients[0]]->gameInfo->color;
				if (newI < 0 || newI > (MAPSIZE - 10) || newJ < 0 || newJ > (MAPSIZE - 10))
				{
					printf("wrong newI newJ\n");
					exit(1);
				}

				info[sfd_clients[0]]->turn = C1;
				info[sfd_clients[1]]->turn = C1;

				map[newI][newJ] = 1;
				info[sfd_clients[1]]->gameInfo->i = newI;
				info[sfd_clients[1]]->gameInfo->j = newJ;
				info[sfd_clients[1]]->gameInfo->color = newColor;
				gameStatus = SEND;
				break;
			}
			case (C2_RCVD):
			{
				printf("current main state: C2_RCVD\n");
				int newI = info[sfd_clients[1]]->gameInfo->i;
				int newJ = info[sfd_clients[1]]->gameInfo->j;
				int newColor = info[sfd_clients[1]]->gameInfo->color;

				info[sfd_clients[0]]->turn = C2;
				info[sfd_clients[1]]->turn = C2;

				map[newI][newJ] = 2;
				info[sfd_clients[0]]->gameInfo->i = newI;
				info[sfd_clients[0]]->gameInfo->j = newJ;
				info[sfd_clients[0]]->gameInfo->color = newColor;
				gameStatus = SEND;
				break;
			}
			case (SEND):
			{
				printf("current main state: SEND\n");
				gameStatus = C1_SEND;
				break;
			}
			case (C1_SENT):
			{
				printf("current main state: C1_SENT\n");
				gameStatus = C2_SEND;
				break;
			}
			case (C2_SENT):
			{
				printf("current main state: C2_SENT\n");
				int newI = info[sfd_clients[1]]->gameInfo->i;
				int newJ = info[sfd_clients[1]]->gameInfo->j;

				printf("before checkGameOver\n");
				if (checkGameOver(map, newI, newJ) == GAMEOVER)
					gameStatus = C1_GAMEOVER;
				else
				{
					if (info[sfd_clients[0]]->turn == C1)
					{
						printf("\nCURRENTLY C1 TURN\n");
						info[sfd_clients[0]]->turn = C2;
						info[sfd_clients[1]]->turn = C2;
						gameStatus = C2_WAITING;
					}
					else if (info[sfd_clients[1]]->turn == C2)
					{
						printf("\nCURRENTLY C2 TURN\n");
						info[sfd_clients[0]]->turn = C1;
						info[sfd_clients[1]]->turn = C1;
						gameStatus = C1_WAITING;
					}
				}
				break;
			}
			case (ENDGAME):
			{
				printf("current main state: ENDGAME\n");
				// freeInfo(info);
				close(sfd_server);

				printf("[%d] terminated\n", getpid());

				return EXIT_SUCCESS;
				break;
			}
		}
	}
	endThread(info);

	freeInfo(info);
	close(sfd_server);

	printf("[%d] terminated\n", getpid());

	return EXIT_SUCCESS;
}


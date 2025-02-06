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

	// if (info[0]->gameInfo == NULL)
	// 	printf("right after startGame, info[0]->gameInfo is NULL\n");
	// else
	// 	printf("right after startGame, info[0]->gameInfo is NOT NULL\n");

	// if (info[1]->gameInfo == NULL)
	// 	printf("right after startGame, info[1]->gameInfo is NULL\n");
	// else
	// 	printf("right after startGame, info[1]->gameInfo is NOT NULL\n");


	for (int i = 0; i < 2; i++)
		printf("-----------this is sfd_clients[%d]:%d\n", i, sfd_clients[i]);

	if (info[sfd_clients[0]] == NULL)
		printf("right after startGame, info[sfd_clients[0]] is NULL\n");
	else
		printf("right after startGame, info[sfd_clients[0]] is NOT NULL\n");

	if (info[sfd_clients[1]] == NULL)
		printf("right after startGame, info[sfd_clients[1]] is NULL\n");
	else
		printf("right after startGame, info[sfd_clients[1]] is NOT NULL\n");

	if (info[sfd_clients[0]]->gameInfo == NULL)
		printf("right after startGame, info[sfd_clients[0]]->gameInfo is NULL\n");
	else
		printf("right after startGame, info[sfd_clients[0]]->gameInfo is NOT NULL\n");

	if (info[sfd_clients[1]]->gameInfo == NULL)
		printf("right after startGame, info[sfd_clients[1]]->gameInfo is NULL\n");
	else
		printf("right after startGame, info[sfd_clients[1]]->gameInfo is NOT NULL\n");

	printf("starting main thread\n");
	while (1)
	{
		//main thread
		// printf("main thread gameStatus:%d\n", gameStatus);
		switch (gameStatus)
		{
			case (C1_RCVD):
			{
				printf("current main state: C1_RCVD\n");
				int newI = info[sfd_clients[0]]->gameInfo->i;
				int newJ = info[sfd_clients[0]]->gameInfo->j;

				map[newI][newJ] = 1;
				info[sfd_clients[1]]->gameInfo->i = newI;
				info[sfd_clients[1]]->gameInfo->j = newJ;
				gameStatus = SEND;
				break;
			}
			case (C1_SENT):
			{
				printf("current main state: C1_SENT\n");
				int newI = info[sfd_clients[0]]->gameInfo->i;
				int newJ = info[sfd_clients[0]]->gameInfo->j;

				if (checkGameOver(map, newI, newJ) == GAMEOVER)
					gameStatus = GAMEOVER;
				else
				{
					info[sfd_clients[0]]->turn = C2;
					info[sfd_clients[1]]->turn = C2;
					gameStatus = C2_WAITING;
				}
				break;
			}

			case (C2_RCVD):
			{
				printf("current main state: C2_RCVD\n");
				int newI = info[sfd_clients[1]]->gameInfo->i;
				int newJ = info[sfd_clients[1]]->gameInfo->j;

				map[newI][newJ] = 0;
				info[sfd_clients[0]]->gameInfo->i = newI;
				info[sfd_clients[0]]->gameInfo->j = newJ;
				gameStatus = SEND;
				break;
			}
			case (C2_SENT):
			{
				printf("current main state: C2_SENT\n");
				int newI = info[sfd_clients[1]]->gameInfo->i;
				int newJ = info[sfd_clients[1]]->gameInfo->j;

				if (checkGameOver(map, newI, newJ) == GAMEOVER)
					gameStatus = GAMEOVER;
				else
				{
					info[sfd_clients[0]]->turn = C1;
					info[sfd_clients[1]]->turn = C1;
					gameStatus = C1_WAITING;
				}
				break;
			}
			case (ENDGAME):
			{
				printf("current main state: ENDGAME\n");
				freeInfo(info);
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


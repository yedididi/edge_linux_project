#include "../incs/define.h"

#define LISTEN_BACKLOG 5
#define MAX_BUF 256

int playerNum = 0;

int main(int argc, char **argv)
{
	int sfd_server = 0;
	struct sockaddr_in addr_server;
	int optval = 1;
	int map[17][17] = {0, };

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
		if (startGame(sfd_server, info, &playerNum) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}

	endThread(info);
	while (1)
	{
		//main thread
		(void)map;

	}

	freeInfo(info);
	close(sfd_server);

	printf("[%d] terminated\n", getpid());

	return EXIT_SUCCESS;
}


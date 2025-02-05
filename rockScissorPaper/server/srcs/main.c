#include "../incs/define.h"

#define LISTEN_BACKLOG 5
#define MAX_BUF 256

int playerNum = 0;

int main(int argc, char **argv)
{
	int sfd_server = 0;
	int sfd_client = 0;
	struct sockaddr_in addr_server;
	struct sockaddr_in addr_client;
	socklen_t addr_client_len = 0;
	int optval = 1;
	pthread_t thread_id = 0;

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
		if (startGame(addr_client_len, sfd_client, addr_client, sfd_server, info, &playerNum, thread_id) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}

	freeInfo(info);
	close(sfd_server);

	printf("[%d] terminated\n", getpid());

	return EXIT_SUCCESS;
}


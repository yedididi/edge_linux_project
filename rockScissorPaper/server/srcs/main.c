#include "../incs/define.h"

#define LISTEN_BACKLOG 5
#define MAX_BUF 256

pid_t pid;
int playerNum = 0;

int main(int argc, char **argv)
{
	int ret;
	int sfd_server, sfd_client;
	struct sockaddr_in addr_server;
	struct sockaddr_in addr_client;
	socklen_t addr_client_len;
	int optval = 1;
	pthread_t thread_id;

	t_info *info[MAX_BUF];

	if(argc != 1) {
		printf("usage: %s\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("[%d] running %s\n", pid = getpid(), argv[0]);

	if (startTCP(&sfd_server, &pid, &addr_server, &optval) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("before fillInfo\n");
	// if (fillInfo(&info, &playerNum) == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);

	printf("before for loop\n");
	for(;;) {
		printf("[%d] waiting for client ...\n", pid);
		addr_client_len = sizeof(addr_client);
		sfd_client = accept(sfd_server, (struct sockaddr *)&addr_client, &addr_client_len);
		if(sfd_client == -1) {
			printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
			return EXIT_FAILURE;
		}
		printf("[%d] connected\n", pid);

		pthread_mutex_lock(&(info[sfd_client]->playerNumMuxtex));
		playerNum++;
		printf("player Num increased to %d\n", playerNum);
		pthread_mutex_unlock(&(info[sfd_client]->playerNumMuxtex));

		printf("[%d] creating thread\n", pid);
		// info->clientfd = sfd_client;
		if (fillInfo(&info[sfd_client], &playerNum, sfd_client) == EXIT_FAILURE)
		return (EXIT_FAILURE);

		ret = pthread_create(&thread_id, NULL, &startGame, info[sfd_client]);
		if(ret != 0) {
			printf("[%d] error: %d (%d)\n", pid, ret, __LINE__);
			return EXIT_FAILURE;
		}
		pthread_detach(thread_id);
	}

	freeInfo(info);
	close(sfd_server);

	printf("[%d] terminated\n", pid);

	return EXIT_SUCCESS;
}


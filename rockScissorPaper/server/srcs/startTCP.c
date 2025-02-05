#include "../incs/define.h"

int startTCP(int *sfd_server, int *pid, struct sockaddr_in *addr_server, int *optval)
{
	*sfd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(*sfd_server == -1) {
		printf("[%d] error: %s (%d)\n", *pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}

	/* to prevent "Address already in use" error */
	int ret = setsockopt(*sfd_server, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(*optval));
	if(ret == -1) {
		printf("[%d] error: %s (%d)\n", *pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}

	memset(addr_server, 0, sizeof(*addr_server));
	addr_server->sin_family = AF_INET;
	addr_server->sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server->sin_port = htons(SERVER_PORT);
	ret = bind(*sfd_server, (struct sockaddr *)addr_server, sizeof(*addr_server));
	if(ret == -1) {
		printf("[%d] error: %s (%d)\n", *pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}

	ret = listen(*sfd_server, LISTEN_BACKLOG);
	if(ret == -1) {
		printf("[%d] error: %s (%d)\n", *pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}
	return (0);
}
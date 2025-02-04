#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "define.h"

#define MAX_BUF 256

pid_t pid;

int main()
{
	int ret;
	int len;
	int sfd;
	struct sockaddr_in addr_server;
	char buf[MAX_BUF];

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1) {
		printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}

	memset(&addr_server, 0, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr_server.sin_port = htons(SERVER_PORT);
	ret = bind(sfd, (struct sockaddr *)&addr_server, sizeof(addr_server));
	if(ret == -1) {
		printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}
	printf("[%d] connected\n", pid);

    write(sfd, "connect", strlen("connect"));

	len = read(sfd, buf, MAX_BUF-1);
	if(len > 0) {
		buf[len] = 0;
		printf("[%d] received: %s\n", pid, buf);

        if(strcmp(buf, "OK") == 0) {
			write(sfd, "Ready", strlen("Ready"));
			printf("[%d] Ready\n", pid);
		}
	}

	close(sfd);

	return EXIT_SUCCESS;
}

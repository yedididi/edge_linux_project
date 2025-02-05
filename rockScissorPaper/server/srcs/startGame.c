#include "../incs/define.h"

int startGame(socklen_t addr_client_len, int sfd_client, struct sockaddr_in addr_client, int sfd_server, t_info **info, int *playerNum, pthread_t thread_id)
{
    printf("[%d] waiting for client ...\n", getpid());
    addr_client_len = sizeof(addr_client);
    sfd_client = accept(sfd_server, (struct sockaddr *)&addr_client, &addr_client_len);
    if(sfd_client == -1) {
        printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
        return EXIT_FAILURE;
    }
    printf("[%d] connected\n", getpid());

    pthread_mutex_lock(&(info[sfd_client]->playerNumMuxtex));
    (*playerNum)++;
    printf("player Num increased to %d\n", *playerNum);
    pthread_mutex_unlock(&(info[sfd_client]->playerNumMuxtex));

    printf("[%d] creating thread\n", getpid());
    // info->clientfd = sfd_client;
    if (fillInfo(&info[sfd_client], playerNum, sfd_client) == EXIT_FAILURE)
    return (EXIT_FAILURE);

    int ret = pthread_create(&thread_id, NULL, &startThread, info[sfd_client]);
    if(ret != 0) {
        printf("[%d] error: %d (%d)\n", getpid(), ret, __LINE__);
        return EXIT_FAILURE;
    }
    pthread_detach(thread_id);
    return (EXIT_SUCCESS);
}
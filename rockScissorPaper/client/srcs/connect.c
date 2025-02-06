#include "../incs/define.h"

int connectToServer()
{
    int ret, sfd;
    struct sockaddr_in addr_server;

    // 소켓 생성
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("error: %s (%d)\n", strerror(errno), __LINE__);
        return EXIT_FAILURE;
    }
    
    // 서버 주소 설정
    memset(&addr_server, 0, sizeof(addr_server));
    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_server.sin_port = htons(SERVER_PORT);
    
    // 서버 연결
    ret = connect(sfd, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if (ret == -1) {
        printf("error: %s (%d)\n", strerror(errno), __LINE__);
        close(sfd);
        return EXIT_FAILURE;
    }
    printf("connected\n");
    return (sfd);
}
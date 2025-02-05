#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "define.h"

#define MAX_BUF 256

typedef struct {
    float x;
    float y;
} Coord;

// 좌표 수신 및 재전송 함수
void* receive_and_send(void* arg) {
    int sock = *(int*)arg;
    char buf[MAX_BUF];
    Coord coord;

    while (1) {
        // 서버에서 좌표 수신
        ssize_t recv_size = recv(sock, buf, sizeof(Coord), 0);
        if (recv_size <= 0) {
            printf("서버 연결 종료 또는 오류 발생\n");
            break;
        }

        memcpy(&coord, buf, sizeof(Coord));
        printf("받은 좌표: X = %.2f, Y = %.2f\n", coord.x, coord.y);

        // 받은 좌표를 다시 서버로 전송
        if (send(sock, &coord, sizeof(Coord), 0) == -1) {
            printf("좌표 전송 실패: %s\n", strerror(errno));
            break;
        }

        printf("좌표 재전송 완료: X = %.2f, Y = %.2f\n", coord.x, coord.y);
    }
    
    close(sock);
    pthread_exit(NULL);
}

int main() {
    int ret, sfd;
    struct sockaddr_in addr_server;
    pthread_t thread_id;
    
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

    // 서버에서 첫 메시지 수신
    char buf[MAX_BUF];
    int len = read(sfd, buf, MAX_BUF - 1);
    if (len > 0) {
        buf[len] = 0;
        printf("서버에서 받은 메시지: %s\n", buf);

        if (strcmp(buf, "OK") == 0) {
            write(sfd, "ready", strlen("ready"));
            printf("ready\n");

            char buf[64];
            int retRead = read(sfd, buf, 63);
            buf[retRead] = '\0';
            
            if (strncmp(buf, "playStart", 10) == 0) {
                printf("done\n");

                // 좌표 수신 및 전송 스레드 생성
                if (pthread_create(&thread_id, NULL, receive_and_send, (void*)&sfd) != 0) {
                    perror("스레드 생성 실패");
                    close(sfd);
                    return EXIT_FAILURE;
                }

                // 스레드 종료 대기
                pthread_join(thread_id, NULL);
            }
        }
    }

    close(sfd);
    return EXIT_SUCCESS;
}

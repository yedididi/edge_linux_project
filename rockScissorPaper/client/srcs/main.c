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
    float i;
    float j;
    int gamestatus;
} Coord;

// 좌표 수신 및 재전송 함수
void* receive_thread(void* arg) {
    int sock = *(int*)arg;
    Coord coord;
    char buf[MAX_BUF];

    while (1) {
        // 서버에서 좌표 수신
        ssize_t recv_size = recv(sock, buf, sizeof(Coord), 0);
        if (recv_size <= 0) {
            printf("서버 연결 종료 또는 오류 발생\n");
            break;
        }

        memcpy(&coord, buf, sizeof(Coord));
        printf("[수신] X = %.2f, Y = %.2f\n", coord.i, coord.j);
    }

    close(sock);
    pthread_exit(NULL);
}

void* send_thread(void* arg) {
    int sock = *(int*)arg;
    Coord coord;

    while (1) {
        // 좌표 입력 받기
        printf("보낼 좌표 입력 (X Y): ");
        if (scanf("%f %f", &coord.i, &coord.j) != 2) {
            printf("잘못된 입력! 다시 입력하세요.\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
            continue;
        }
        usleep(100);
        // 서버로 좌표 전송
        if (send(sock, &coord, sizeof(Coord), 0) == -1) {
            printf("좌표 전송 실패: %s\n", strerror(errno));
            break;
        }

        printf("[발신] X = %.2f, Y = %.2f\n", coord.i, coord.j);
    }

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int ret, sfd;
    struct sockaddr_in addr_server;
    pthread_t recv_tid, send_tid;
    
    // 소켓 생성
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("error: %s (%d)\n", strerror(errno), __LINE__);
        printf("1");
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
        printf("2");
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
                if (pthread_create(&recv_tid, NULL, receive_thread, (void*)&sfd) != 0) {
                    perror("수신 스레드 생성 실패");
                    close(sfd);
                    return EXIT_FAILURE;
                }

                // 발신 스레드 생성
                if (pthread_create(&send_tid, NULL, send_thread, (void*)&sfd) != 0) {
                    perror("발신 스레드 생성 실패");
                    close(sfd);
                    return EXIT_FAILURE;
                }

                // 스레드 종료 대기
                pthread_join(recv_tid, NULL);
                pthread_join(send_tid, NULL);
            }
        }
    }

    close(sfd);
    return EXIT_SUCCESS;
}

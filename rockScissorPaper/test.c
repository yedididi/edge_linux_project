#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "define.h"

#define MAX_BUF 256
#define TOUCH_DEVICE "/dev/input/event2"

typedef struct {
    float i;  // X 좌표
    float j;  // Y 좌표
    int gamestatus; // 게임 상태 (필요 시 활용)
} Coord;

// 전역 변수 및 동기화 객체
Coord touch_coord = {0, 0, 0};
pthread_mutex_t lock;

// 서버에서 좌표 수신하는 스레드
void* receive_thread(void* arg) {
    int sock = *(int*)arg;
    Coord coord;
    char buf[MAX_BUF];

	(void)arg;
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

// 터치 이벤트를 처리하는 스레드
void* touch_thread(void* arg) {
    int fd, ret;
    struct input_event ev;

	(void)arg;
    // 터치스크린 장치 열기
    fd = open(TOUCH_DEVICE, O_RDONLY);
    if (fd == -1) {
        printf("터치 장치 열기 실패: %s (%d)\n", strerror(errno), __LINE__);
        pthread_exit(NULL);
    }
    printf("터치 이벤트 감지 시작: %s\n", TOUCH_DEVICE);

    while (1) {
        ret = read(fd, &ev, sizeof(struct input_event));
        if (ret == -1) {
            printf("터치 이벤트 읽기 실패: %s (%d)\n", strerror(errno), __LINE__);
            break;
        }

        // X 좌표 감지
        if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X) {
            pthread_mutex_lock(&lock);
            touch_coord.i = ev.value;  // X 좌표 업데이트
            pthread_mutex_unlock(&lock);
        }

        // Y 좌표 감지
        if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y) {
            pthread_mutex_lock(&lock);
            touch_coord.j = ev.value;  // Y 좌표 업데이트
            pthread_mutex_unlock(&lock);
        }

        // 터치 이벤트가 감지되면 출력
        if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 1) {
            pthread_mutex_lock(&lock);
            printf("[터치 감지] X = %.2f, Y = %.2f\n", touch_coord.i, touch_coord.j);
            pthread_mutex_unlock(&lock);
        }
    }

    close(fd);
    pthread_exit(NULL);
}

// 서버로 좌표를 전송하는 스레드
void* send_thread(void* arg) {
    int sock = *(int*)arg;
    Coord coord;

    while (1) {
        sleep(1); // 1초마다 좌표 전송

        pthread_mutex_lock(&lock);
        coord = touch_coord;  // 최신 좌표 가져오기
        pthread_mutex_unlock(&lock);

        // 좌표가 변경되었을 때만 서버로 전송
        if (coord.i > 0 && coord.j > 0) {
            if (send(sock, &coord, sizeof(Coord), 0) == -1) {
                printf("좌표 전송 실패: %s\n", strerror(errno));
                break;
            }
            printf("[발신] X = %.2f, Y = %.2f\n", coord.i, coord.j);
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int ret, sfd;
    struct sockaddr_in addr_server;
    pthread_t recv_tid, send_tid, touch_tid;
    char buf[MAX_BUF];
    
    // 뮤텍스 초기화
    pthread_mutex_init(&lock, NULL);

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
	
	
    int retRead = read(sfd, buf, 63);
    buf[retRead] = '\0';
    
    if (strncmp(buf, "playStart", 10) == 0) {
    printf("게임 시작\n");

                // 수신 스레드 생성
                if (pthread_create(&recv_tid, NULL, receive_thread, (void*)&sfd) != 0) {
                    perror("수신 스레드 생성 실패");
                    close(sfd);
                    return EXIT_FAILURE;
                }

                // 터치 입력 스레드 생성
                if (pthread_create(&touch_tid, NULL, touch_thread, NULL) != 0) {
                    perror("터치 스레드 생성 실패");
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
                pthread_join(touch_tid, NULL);
                pthread_join(send_tid, NULL);
            }
        

    // 뮤텍스 해제
    pthread_mutex_destroy(&lock);
    close(sfd);
    return EXIT_SUCCESS;
}
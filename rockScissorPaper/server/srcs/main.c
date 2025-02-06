#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 25000
#define MAX_BUF 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUF];

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 3. 바인딩
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. 클라이언트 연결 대기
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    // 5. 클라이언트 연결 수락
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n");

    // 6. 클라이언트에서 메시지 수신
    int len = read(client_fd, buffer, MAX_BUF - 1);
    if (len > 0) {
        buffer[len] = '\0';
        printf("Received from client: %s\n", buffer);

        // "connect" 메시지 확인 후 "OK" 응답 전송
        if (strcmp(buffer, "connect") == 0) {
            write(client_fd, "OK", strlen("OK"));
            printf("Sent: OK\n");

            // 클라이언트가 "Ready"를 보낼 때까지 기다림
            len = read(client_fd, buffer, MAX_BUF - 1);
            if (len > 0) {
                buffer[len] = '\0';
                printf("Received from client: %s\n", buffer);

                // 클라이언트가 "Ready"를 보냈는지 확인
                if (strcmp(buffer, "Ready") == 0) {
                    printf("Client is Ready!\n");
                }
            }
        }
    }

    // 7. 클라이언트의 원래 메시지 수신
    len = read(client_fd, buffer, MAX_BUF - 1);
    if (len > 0) {
        buffer[len] = '\0';
        printf("Client Message: %s\n", buffer);

        // 클라이언트에게 응답 전송
        write(client_fd, "Message received", strlen("Message received"));
    }

    // 8. 연결 종료
    close(client_fd);
    close(server_fd);
    printf("Server terminated.\n");

    return 0;
}

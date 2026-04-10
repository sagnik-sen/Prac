// udp_time_client.c

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {

    int sockfd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE] = "TIME_REQUEST";
    socklen_t addr_len = sizeof(addr);

    // ===== TEMPLATE START =====
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // Send time request
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr*)&addr, sizeof(addr));

    // Receive time from server
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     NULL, NULL);

    buffer[n] = '\0';

    printf("Current Server Time: %s\n", buffer);

    close(sockfd);

    return 0;
}
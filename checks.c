// server.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int data[20], n, sum = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    listen(sockfd, 3);

    printf("Waiting for connection...\n");

    new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // receive n
    read(new_socket, &n, sizeof(n));

    // receive data + checksum
    read(new_socket, data, sizeof(data));

    printf("Received data: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", data[i]);
        sum += data[i];
    }

    // carry addition (8-bit)
    while(sum >> 8) {
        sum = (sum & 0xFF) + (sum >> 8);
    }

    if((~sum & 0xFF) == 0) {
        printf("\nNo Error Detected\n");
    } else {
        printf("\nError Detected\n");
    }

    close(new_socket);
    close(sockfd);

    return 0;
}
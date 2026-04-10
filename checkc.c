// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    int data[20], n, sum = 0, checksum;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter data: ");
    for(int i = 0; i < n; i++) {
        scanf("%d", &data[i]);
        sum += data[i];
    }

    // carry addition (8-bit)
    while(sum >> 8) {
        sum = (sum & 0xFF) + (sum >> 8);
    }

    checksum = ~sum & 0xFF;

    data[n] = checksum;
    n++; // include checksum

    printf("Checksum: %d\n", checksum);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // send n
    write(sock, &n, sizeof(n));

    // send data + checksum
    write(sock, data, sizeof(data));

    close(sock);

    return 0;
}

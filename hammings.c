// server_hamming.c

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int bit(char c) { return c - '0'; }
char flip(char c) { return (c == '0') ? '1' : '0'; }

int main() {

    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(addr);

    // ===== SOCKET TEMPLATE =====
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    client_fd = accept(server_fd, (struct sockaddr*)&addr, &addr_len);

    int n = recv(client_fd, buffer, BUFFER_SIZE, 0);
    buffer[n] = '\0';

    printf("Received Codeword: %s\n", buffer);

    // ===== HAMMING LOGIC START =====

    char code[12]; // 1-based indexing
    for(int i = 1; i <= 11; i++)
        code[i] = buffer[i-1];

    int s1 = bit(code[1]) ^ bit(code[3]) ^ bit(code[5]) ^
             bit(code[7]) ^ bit(code[9]) ^ bit(code[11]);

    int s2 = bit(code[2]) ^ bit(code[3]) ^ bit(code[6]) ^
             bit(code[7]) ^ bit(code[10]) ^ bit(code[11]);

    int s4 = bit(code[4]) ^ bit(code[5]) ^
             bit(code[6]) ^ bit(code[7]);

    int s8 = bit(code[8]) ^ bit(code[9]) ^
             bit(code[10]) ^ bit(code[11]);

    int error_pos = s8*8 + s4*4 + s2*2 + s1;

    if(error_pos == 0) {
        printf("No error detected.\n");
    } else {
        printf("Error detected at position: %d\n", error_pos);
        code[error_pos] = flip(code[error_pos]);
        printf("Corrected bit at position %d.\n", error_pos);
    }

    // Build corrected string
    char corrected[12];
    for(int i = 1; i <= 11; i++)
        corrected[i-1] = code[i];
    corrected[11] = '\0';

    printf("Corrected Codeword: %s\n", corrected);

    // ===== END LOGIC =====

    close(client_fd);
    close(server_fd);

    return 0;
}
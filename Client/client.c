#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Функция для отправки данных на сервер
void send_data(int client_socket) {
    // Создание RRC Connection Setup Request и отправка на сервер
    char message[] = "RRC Connection Setup Request";
    send(client_socket, message, strlen(message), 0);

    printf("RRC Connection Setup Request отправлен на сервер.\n");
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка при создании сокета");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при установлении соединения с сервером");
        return 1;
    }

    send_data(client_socket);

    close(client_socket);

    return 0;
}
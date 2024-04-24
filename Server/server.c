#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Функция для приема данных от клиента и отправки RRC Connection Setup Complete
void handle_client(int client_socket) {
    char buffer[1024] = {0};
    
    // Прием данных от клиента (RRC Connection Setup Request)
    recv(client_socket, buffer, 1024, 0);
    printf("Получено от клиента: %s\n", buffer);

    // Отправка RRC Connection Setup Complete клиенту
    char response[] = "RRC Connection Setup Complete";
    send(client_socket, response, strlen(response), 0);

    printf("RRC Connection Setup Complete отправлен клиенту.\n");
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка при создании сокета");
        return 1;
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при привязке сокета к адресу");
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("Ошибка при прослушивании порта");
        return 1;
    }

    printf("Сервер ожидает подключения...\n");

    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Ошибка при принятии подключения");
        return 1;
    }

    handle_client(client_socket);

    close(client_socket);
    close(server_socket);

    return 0;
}
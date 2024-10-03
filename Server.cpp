#include "Chat.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main() {
    socket_file_description = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_description == -1) {
        cout << "Socket creation failed.!" << endl;
        exit(1);
    }
    // 
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind_status = bind(socket_file_description, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        exit(1);
    }
    // Поставим сервер на прием данных 
    connection_status = listen(socket_file_description, 5);
    if (connection_status == -1) {
        cout << "Socket is unable to listen for new connections.!" << endl;
        exit(1);
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }
    length = sizeof(client);
    connection = accept(socket_file_description, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        cout << "Server is unable to accept the data from client.!" << endl;
        exit(1);
    }

    while (1) {
        bzero(message, BUFFER);
        read(connection, message, sizeof(message));
        if (strncmp("Client ending.", message, 13) == 0) {
            cout << "Client Exited." << endl;
            cout << "Server is Exiting..!" << endl;
            break;
        }
        cout << "Data received from client: " << message << endl;
        bzero(message, BUFFER);
        cout << "Enter the message you want to send to the client: " << endl;
        cin >> message;
        ssize_t bytes = write(connection, message, sizeof(message));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            cout << "Data successfully sent to the client.!" << endl;
        }
    }
    // закрываем сокет, завершаем соединение
    close(socket_file_description);


    return 0;
}
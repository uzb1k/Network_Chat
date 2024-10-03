#include "Chat.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;


int main() {
    Chat c;
    User* user;
    bool WhileChatting = true;
    int choose;
    string login, password;

    socket_file_description = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_description == -1) {
        cout << "Creation of Socket failed!" << endl;
        exit(1);
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Установим соединение с сервером
    connection = connect(socket_file_description, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with the server failed.!" << endl;
        exit(1);
    }
    else {
        cout << "Connection complete." << endl;
    }

    while (WhileChatting) {
        try {
            cout << "What can we do for you? (1 - Register. 2 - Login in. 3 - Exit.) " << endl;
            cin >> choose;

            if (choose != 1 && choose != 2 && choose != 3) { //Проверка пользователя на корректный ввод
                throw "Incorrect option"; //И перехват исключения в случае ошибки
            }

            switch (choose) {
            case 1:
                c.Register();
                break;

            case 2:
                if (c.GetUsers().empty()) { //Проверяем наличие пользователей и останавливаем вход в случае их отсутсвия
                    cout << "Currently, there are no registered users." << endl;
                    break;
                }

                cout << "Enter your login: " << endl;
                cin >> login;

                cout << "Enter your password: " << endl;
                cin >> password;
                user = c.Authenticate(login, password);

                if (user != nullptr) {
                    cout << "Welcome! " << user->GetLogin() << endl;
                    bool chatting = true;
                    while (chatting) {
                        try {
                            bzero(message, sizeof(message));
                            cout << "Enter your message to the server: " << endl;

                            cin.ignore();
                            fgets(message, sizeof(message), stdin);

                            c.SendMessage(login, message);

                            cout << "Waiting for the answer from the server" << endl;
                            bzero(message, sizeof(message));
                            read(socket_file_description, message, sizeof(message));
                            cout << "Data received from server: " << message << endl;

                            cout << "Do you want to continue chatting? 1 - yes, 2 - no" << endl;
                            cin >> choose;

                            if (choose != 1 && choose != 2) {
                                throw "Incorrect option";
                            }
                            if (choose != 1) {
                                chatting = false;
                            }


                        }
                        //Перехват исключений
                        catch (const char* e) {
                            cout << "Exeption caught: " << e << endl;

                            cin.clear();
                            cin.ignore(99999, '\n');

                            continue;
                        }
                    }
                }
                break;

            case 3:
                cout << "Exiting...";
                char ending[] = "Client ending.";

                write(socket_file_description, ending, sizeof(ending));
                return 0;
            }
        }
        //Ещё один перехват для каждого switch
        catch (const char* e) {
            cout << "Exeption caught: " << e << endl;

            cin.clear();
            cin.ignore(99999, '\n');

            continue;
        }

    }
}
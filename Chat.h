#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>


using namespace std;

#define PORT 7777
#define BUFFER 1024

extern struct sockaddr_in serveraddress, client;
extern socklen_t length;
extern int socket_file_description, connection, bind_status, connection_status;

extern char message[BUFFER];

class User {
public:
    User();
    User(string login, string password);
    ~User();

    string GetLogin();
    void SetLogin(const string& login);

    string GetPassword();
    void SetPassword(const string& password);

    bool CheckCredentials(const string& login, const string& password);
private:
    string _login, _password;
};



class Chat {
public:
    void AddUser(string login, string password);
    void Register();

    vector<User>& GetUsers();
    User* Authenticate(const string& login, const string& password);

    void SendMessage(string login, string message);

private:
    vector<User> users;
};
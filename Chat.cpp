#include "Chat.h"

sockaddr_in serveraddress, client;
socklen_t length;
int socket_file_description, connection, bind_status, connection_status;

char message[BUFFER];

User::User() {}

User::User(string login, string password) : _login(login), _password(password) {}

User::~User() {}

//Добавление сеттеров и геттеров
void User::SetLogin(const string& login) {
    _login = login;
}
string User::GetLogin() {
    return _login;
}

void User::SetPassword(const string& password) {
    _password = password;
}
string User::GetPassword() {
    return _password;
}


bool User::CheckCredentials(const string& login, const string& password) {
    return(_login == login && _password == password);
}





void Chat::Register() {
    string login, password;

    cout << "Enter your new Login: " << endl;
    cin >> login;

    cout << "Enter your new Password: " << endl;
    cin >> password;

    User newUser(login, password);
    AddUser(newUser.GetLogin(), newUser.GetPassword());

    cout << "User has registered!" << endl;
}


vector<User>& Chat::GetUsers() {
    return users;
}

User* Chat::Authenticate(const string& login, const string& password) {
    for (User& user : users) {
        if (user.CheckCredentials(login, password)) {
            cout << "User authenticated successfully." << endl;
            return &user;
        }
    }
    cout << "Invalid Login or Password!" << endl;
    return nullptr;
}

void Chat::SendMessage(string login, string message) {
    char buffer[BUFFER];

    string fullMessage = login + ": " + message;

    bzero(buffer, BUFFER);
    ssize_t bytes = write(socket_file_description, fullMessage.c_str(), fullMessage.size());
    if (bytes >= 0) {
        cout << "Data sent to the server successfully." << endl;
    }
}

void Chat::AddUser(string login, string password) {
    users.emplace_back(login, password);
}


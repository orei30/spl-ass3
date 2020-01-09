#ifndef CLIENT_
#define CLIENT_

#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <connectionHandler.h>
#include <book.h>
#include <ineventory.h>
#include <STOMPMessage.h>

using namespace std;

class Client {
    private:
        string _userName;
        bool _connected;
        vector<string> _reciepts;
        ConnectionHandler* _connectionHandler;
        Inventory _inventory;
    public:
        Client(string);
        ~Client();
        string getUserName();
        void setUserName(string);
        void sendDataToServer();
        void getDataFromServer();
        int newReciept(string);
        void login();
        void joinGenre();
        void exitGenre();
        void addBook();
        void borrowBook();
        void returnBook();
        void genreStatus();
        void logout();
        void messageRecieved(STOMPMessage);
};
#endif
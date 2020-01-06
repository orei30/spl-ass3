#ifndef CLIENT_
#define CLIENT_

#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <connectionHandler.h>

using namespace std;

class Client {
    private:
        string _userName;
        bool _connected;
        unordered_map<string, int> _subscriptions;
        int _subscriptionId;
        vector<string> _reciepts;
        ConnectionHandler* _connectionHandler;
        unordered_map<string, string> _bookLenders;
    public:
        Client(string);
        ~Client();
        string getUserName();
        void setUserName(string);
        void sendDataToServer();
        void getDataFromServer();
        int subscribe(string);
        int newReciept();
        void updateReciept(int, string);
        void lenderBook(string, string);
        string getLender(string);
        void login();
        void joinGenre();
        void exitGenre();
        void addBook();
        void boroowBook();
        void returnBook();
        void genreStatus();
        void logout();
        // void messageRecieved(STOMPMessage);
};
#endif
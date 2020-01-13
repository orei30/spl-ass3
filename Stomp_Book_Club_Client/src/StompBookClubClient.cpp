#include <stdlib.h>
#include <connectionHandler.h>
#include <client.h>
#include <STOMPMessage.h>

vector<thread> threads;

int main (int argc, char *argv[]) {
    Client* client = new Client();
    thread th1(&Client::sendDataToServer, client);
    thread th2(&Client::getDataFromServer, client);
    th1.join();
    th2.join();
    delete client;
    return 0;
}
//TODO: there mamroy leak if user logged in twice
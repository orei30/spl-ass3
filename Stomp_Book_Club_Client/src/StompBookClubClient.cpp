#include <stdlib.h>
#include <connectionHandler.h>
#include <client.h>
#include <STOMPMessage.h>

int main (int argc, char *argv[]) {
    Client task1("");
    thread th1(&Client::sendDataToServer, &task1);
    thread th2(&Client::getDataFromServer, &task1);
    th1.join();
    th2.join();
    return 0;
}
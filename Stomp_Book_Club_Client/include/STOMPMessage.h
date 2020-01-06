#ifndef STOMP_MESSAGE_
#define STOMP_MESSAGE_

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class STOMPMessage {
    private:
        string _command;
        unordered_map<string, string> _headers;
        string _body;
    public:
        STOMPMessage();
        void setCommand(string command);
        string getCommand();
        void addHeader(string, string);
        string getHeader(string);
        void addBody(string);
        string getBody();
        string get();
};
#endif
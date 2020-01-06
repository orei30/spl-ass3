#include <STOMPMessage.h>

STOMPMessage::STOMPMessage() : _command(""), _headers(), _body("") {};

void STOMPMessage::setCommand(string command) {
    _command = command;
}

string STOMPMessage::getCommand() {
    return _command;
}

void STOMPMessage::addHeader(string headerName, string headerValue) {
    _headers.insert({headerName,headerValue});
}

string STOMPMessage::getHeader(string title) {
    return _headers.find(title)->first;
}

void STOMPMessage::addBody(string body) {
    _body = body;
}

string STOMPMessage::getBody() {
    return _body;
}

string STOMPMessage::get() {
    string headersString = "";
    for (pair<string, string> header : _headers)
        headersString = headersString + header.first + ":" + header.second + "\n";
    return _command + "\n" + headersString + _body + "\n^@";
}


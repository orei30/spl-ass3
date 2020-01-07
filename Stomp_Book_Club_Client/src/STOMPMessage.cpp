#include <STOMPMessage.h>

STOMPMessage::STOMPMessage() : _command(""), _headers(), _body("") {};

STOMPMessage::STOMPMessage(string frame) {
    istringstream streamFrame(frame);
    int lineIndex = 0;
    std::string line;
    while (getline(streamFrame, line))
    {
        if (lineIndex == 0)
            {
                setCommand(line);
                lineIndex = lineIndex + 1;
            }
            else
            {
                bool header = true;
                if (line == "") {
                    header = false;
                    lineIndex = lineIndex + 1;
                }
                if (header)
                {
                    int colonIndex = line.find(":");
                    string title = line.substr(0, colonIndex);
                    string value = line.substr(colonIndex + 1);
                    addHeader(title, value);
                    lineIndex = lineIndex + 1;
                }
                else
                {
                    if (line != "^@")
                    {
                        addBody(line);
                        lineIndex = lineIndex + 1;
                    }
                    else {
                        lineIndex = 0;
                    }
                }
            }
    }
}

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


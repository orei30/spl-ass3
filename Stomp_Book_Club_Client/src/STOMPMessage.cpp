#include <STOMPMessage.h>

STOMPMessage::STOMPMessage() : _command(""), _headers(), _body("") {};

STOMPMessage::STOMPMessage(string frame) : _command(""), _headers(), _body("") 
{
    istringstream streamFrame(frame);
    int lineIndex(0);
    std::string line;
    bool header = true;
    while (getline(streamFrame, line))
    {
        if (lineIndex == 0)
        {
            setCommand(line);
            lineIndex = lineIndex + 1;
        }
        else
        {
            if (line.empty())
            {
                header = false;
                lineIndex = lineIndex + 1;
            }
            if (header)
            {
                int colonIndex(line.find(":"));
                string title(line.substr(0, colonIndex));
                string value(line.substr(colonIndex + 1));
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
                else
                {
                    lineIndex = 0;
                }
            }
        }
    }
}

void STOMPMessage::setCommand(string command)
{
    _command = command;
}

string STOMPMessage::getCommand()
{
    return _command;
}

void STOMPMessage::addHeader(string headerName, string headerValue)
{
    _headers.insert({headerName, headerValue});
}

string STOMPMessage::getHeader(string title)
{
    return _headers.find(title)->second;
}

void STOMPMessage::addBody(string body)
{
    _body = body;
}

string STOMPMessage::getBody()
{
    return _body;
}

string STOMPMessage::get()
{
    string headersString("");
    for (pair<string, string> header : _headers)
        headersString = headersString + header.first + ":" + header.second + "\n";
    if (_body == "")
        return _command + "\n" + headersString + "\n^@";
    else
        return _command + "\n" + headersString + "\n" + _body + "\n^@";
}

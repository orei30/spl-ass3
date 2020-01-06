#include <stdlib.h>
#include <connectionHandler.h>
#include <client.h>
#include <STOMPMessage.h>

// Client::Client(ConnectionHandler &connectionHandler) :_userName(""), _subscriptions(), _subscriptionId(1), _reciepts(), _connectionHandler(connectionHandler), _bookLenders() {}

Client::Client(string username) : _connected(false), _userName(username), _subscriptions(), _subscriptionId(1), _reciepts(), _connectionHandler(), _bookLenders() {}

Client::~Client()
{
    delete _connectionHandler;
}

string Client::getUserName()
{
    return _userName;
}

void Client::setUserName(string userName)
{
    _userName = userName;
}

int Client::subscribe(string genre)
{
    _subscriptionId = _subscriptionId + 1;
    if (_subscriptions.find(genre) == _subscriptions.end())
    {
        _subscriptions.insert({genre, _subscriptionId});
        return _subscriptionId;
    }
    else
    {
        return _subscriptions.find(genre)->second;
    }
}

int Client::newReciept()
{
    _reciepts.push_back("");
    return _reciepts.size();
}

void Client::updateReciept(int recieptId, string reciept)
{
    _reciepts[recieptId - 1] = reciept;
}

void Client::lenderBook(string bookName, string lender)
{
    _bookLenders.insert({bookName, lender});
}
string Client::getLender(string bookName)
{
    return _bookLenders.find(bookName)->second;
}

void Client::sendDataToServer()
{
    string command;
    cin >> command;
    while (true)
    {
        if (command == "login")
            login();
        if (command == "join")
            joinGenre();
        if (command == "exit")
            exitGenre();
        if (command == "add")
            addBook();
        if (command == "borrow")
            boroowBook();
        if (command == "return")
            returnBook();
        if (command == "status")
            genreStatus();
        if (command == "logout")
            logout();
    }
}

void Client::getDataFromServer()
{
    int lineIndex = 0;
    while (_connected)
    {
        const short bufsize = 1024;
        string answer;
        if (!_connectionHandler->getLine(answer))
        {
            cout << "Disconnected. Exiting...\n" << endl;
            break;
        }
        STOMPMessage message;
        if (lineIndex == 0)
        {
            message.setCommand(answer);
            lineIndex = lineIndex + 1;
        }
        else
        {
            bool header = true;
            if (answer == "")
                header = false;
            if (header)
            {
                int colonIndex = answer.find(":");
                string title = answer.substr(0, colonIndex);
                string value = answer.substr(colonIndex + 1);
                message.addHeader(title, value);
                lineIndex = lineIndex + 1;
            }
            else
            {
                if (answer != "^@")
                {
                    message.addBody(answer);
                    lineIndex = lineIndex + 1;
                }
                else
                    lineIndex = 0;
            }
        }
        cout << message.get() << endl;
        // messageHandler(message);
    }
}

void Client::login()
{
    string hostAndPort, username, password;
    cin >> hostAndPort >> username >> password;
    setUserName(username);
    int colonIndex = hostAndPort.find(":");
    string host = hostAndPort.substr(0, colonIndex);
    short port = stoi(hostAndPort.substr(colonIndex + 1));
    _connectionHandler = new ConnectionHandler(host, port);
    if (!_connectionHandler->connect())
    {
        cerr << "Could not connect to server" << endl;
        return;
    }
    _connected = true;
    STOMPMessage stompMessage;
    stompMessage.setCommand("CONNECT");
    stompMessage.addHeader("accept-version", "1.2");
    stompMessage.addHeader("host", host);
    stompMessage.addHeader("login", username);
    stompMessage.addHeader("passcode", password);
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::joinGenre()
{
    string genre;
    cin >> genre;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SUBSCRIBE");
    stompMessage.addHeader("destination", genre);
    stompMessage.addHeader("id", to_string(subscribe(genre)));
    stompMessage.addHeader("receipt", to_string(newReciept()));
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::exitGenre()
{
    string genre;
    cin >> genre;
    STOMPMessage stompMessage;
    stompMessage.setCommand("UNSUBSCRIBE");
    stompMessage.addHeader("id", to_string(subscribe(genre)));
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::addBook()
{
    string genre, bookName;
    cin >> genre >> bookName;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody(getUserName() + " had added the book " + bookName);
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::boroowBook()
{
    string genre, bookName;
    cin >> genre >> bookName;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody(getUserName() + " wish to borrow " + bookName);
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::returnBook()
{
    string genre, bookName;
    cin >> genre >> bookName;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody("return " + bookName + " to " + getLender(bookName));
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::genreStatus()
{
    string genre;
    cin >> genre;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody("book status");
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::logout()
{
    STOMPMessage stompMessage;
    stompMessage.setCommand("DISCONNECT");
    stompMessage.addHeader("receipt", to_string(newReciept()));
    string message(stompMessage.get());
    const short bufsize = 1024;
    if (!_connectionHandler->sendFrameAscii(message, bufsize))
    {
        cout << "Could not connect to server" << endl;
    }
}

// void Client::messageRecieved(STOMPMessage message) {
//     if(message.getCommand() == "CONNECTED") {

//     }
//     if(message.getCommand() == "MESSAGE") {

//     }
//     if(message.getCommand() == "RECEIPT") {

//     }
//     if(message.getCommand() == "ERROR") {
//         _connected = false;
//     }
// }
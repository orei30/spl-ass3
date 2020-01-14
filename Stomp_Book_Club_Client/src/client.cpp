#include <stdlib.h>
#include <connectionHandler.h>
#include <client.h>

Client::Client() : _userName(), _connected(false), _reciepts(), _connectionHandler(), _inventory(), _terminate(false) {}

Client::Client(const Client &other) : _userName(other._userName), _connected(other._connected), _reciepts(other._reciepts), _connectionHandler(other._connectionHandler), _inventory(other._inventory), _terminate(other._terminate) {}

Client &Client::operator=(const Client &other)
{
    if (this == &other)
    {
        return *this;
    }
    delete _connectionHandler;
    _userName = other._userName;
    _connected = other._connected;
    _reciepts = other._reciepts;
    _connectionHandler = other._connectionHandler;
    _inventory = other._inventory;
    _terminate = other._terminate;
    return *this;
}

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

int Client::newReciept(string receipt)
{
    _reciepts.push_back(receipt);
    return _reciepts.size();
}

void Client::sendDataToServer()
{
    while (!_terminate)
    {
        string command;
        cin >> command;
        if (command == "login")
            login();
        if (command == "join")
            joinGenre();
        if (command == "exit")
            exitGenre();
        if (command == "add")
            addBook();
        if (command == "borrow")
            borrowBook();
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
    while (!_terminate)
    {
        if (_connected)
        {
            string frame;
            if (!_connectionHandler->getFrameAscii(frame, '\0'))
            {
                cout << "Disconnected. Exiting...\n"
                     << endl;
                break;
            }
            STOMPMessage message(frame);
            cout << message.get() << endl;
            messageRecieved(message);
        }
    }
}

void Client::login()
{
    string hostAndPort, username, password;
    cin >> hostAndPort >> username >> password;
    setUserName(username);
    int colonIndex(hostAndPort.find(":"));
    string host(hostAndPort.substr(0, colonIndex));
    short port(stoi(hostAndPort.substr(colonIndex + 1)));
    if (!_connected)
    {
        if (_connectionHandler != nullptr)
        {
            delete _connectionHandler;
            _connectionHandler = nullptr;
        }
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
        if (!_connectionHandler->sendFrameAscii(message, '\0'))
        {
            cout << "Could not connect to server" << endl;
        }
    }
}
void Client::joinGenre()
{
    string genre;
    cin >> genre;
    STOMPMessage stompMessage;
    stompMessage.setCommand("SUBSCRIBE");
    stompMessage.addHeader("destination", genre);
    stompMessage.addHeader("id", to_string(_inventory.initializeGenre(genre)));
    stompMessage.addHeader("receipt", to_string(newReciept("#" + genre)));
    string message(stompMessage.get());
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
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
    stompMessage.addHeader("id", to_string(_inventory.getGenreId(genre)));
    stompMessage.addHeader("receipt", to_string(newReciept("&" + genre)));
    string message(stompMessage.get());
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::addBook()
{
    string genre, bookName;
    cin >> genre;
    getline(cin, bookName);
    bookName.erase(0, 1);
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody(getUserName() + " had added the book " + bookName);
    string message(stompMessage.get());
    cout<<message<< endl;
    _inventory.addBookToGenre(genre, new Book(bookName, getUserName(), genre, "valid"));
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
}
void Client::borrowBook()
{
    string genre, bookName;
    cin >> genre;
    getline(cin, bookName);
    bookName.erase(0, 1);
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    stompMessage.addBody(getUserName() + " wish to borrow " + bookName);
    string message(stompMessage.get());
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
    _inventory.addBookToGenre(genre, new Book(bookName, "", genre, "waiting"));
}
void Client::returnBook()
{
    string genre, bookName;
    cin >> genre;
    getline(cin, bookName);
    bookName.erase(0, 1);
    STOMPMessage stompMessage;
    stompMessage.setCommand("SEND");
    stompMessage.addHeader("destination", genre);
    Book *book = _inventory.getBook(genre, bookName);
    stompMessage.addBody("Returning " + bookName + " to " + book->getLender());
    string message(stompMessage.get());
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
    _inventory.removeBookFromGenre(genre, book);
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
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
}

void Client::logout()
{
    STOMPMessage stompMessage;
    stompMessage.setCommand("DISCONNECT");
    stompMessage.addHeader("receipt", to_string(newReciept("#logout")));
    string message(stompMessage.get());
    if (!_connectionHandler->sendFrameAscii(message, '\0'))
    {
        cout << "Could not connect to server" << endl;
    }
    _terminate = true;
}

void Client::messageRecieved(STOMPMessage message)
{
    if (message.getCommand() == "CONNECTED")
    {
        cout << "Login successful" << endl;
    }
    if (message.getCommand() == "MESSAGE")
    {
        string genre(message.getHeader("destination"));
        string messageBody(message.getBody());
        if (messageBody.find("borrow") != string::npos)
        {
            auto index = messageBody.find_last_of(' ');
            string bookName(messageBody.substr(++index));
            if (_inventory.bookValid(genre, bookName))
            {
                STOMPMessage stompMessage;
                stompMessage.setCommand("SEND");
                stompMessage.addHeader("destination", genre);
                stompMessage.addBody(getUserName() + " has " + bookName);
                string message(stompMessage.get());
                if (!_connectionHandler->sendFrameAscii(message, '\0'))
                {
                    cout << "Could not connect to server" << endl;
                }
            }
        }
        if (messageBody.find("has") != string::npos)
        {
            auto index = messageBody.find_last_of(' ');
            string bookName(messageBody.substr(++index));
            Book *book = _inventory.getBook(genre, bookName);
            if (book != nullptr && book->getStatus() == "waiting")
            {
                book->setStatus("taking"); //TODO: syncronized it
                size_t space_pos = messageBody.find(" ");
                string userName = messageBody.substr(0, space_pos);
                STOMPMessage stompMessage;
                stompMessage.setCommand("SEND");
                stompMessage.addHeader("destination", genre);
                stompMessage.addBody("Taking " + bookName + " from " + userName);
                string message(stompMessage.get());
                if (!_connectionHandler->sendFrameAscii(message, '\0'))
                {
                    cout << "Could not connect to server" << endl;
                }
            }
        }
        if (messageBody.find("Taking") != string::npos)
        {
            int index = messageBody.find_last_of(' ');
            string userName(messageBody.substr(++index));
            string bookName = messageBody.substr(7, messageBody.size() - 13 - userName.size());
            Book *book = _inventory.getBook(genre, bookName);
            if (userName == getUserName())
            {
                book->setStatus("borrowed");
            }
            if (book != nullptr && book->getStatus() == "taking")
            {
                book->setLender(userName);
                book->setStatus("valid");
            }
        }
        if (messageBody.find("Returning") != string::npos)
        {
            auto index = messageBody.find_last_of(' ');
            string userName(messageBody.substr(++index));
            string bookName = messageBody.substr(10, messageBody.size() - 14 - userName.size());
            if (userName == getUserName())
                _inventory.getBook(genre, bookName)->setStatus("valid");
        }
        if (messageBody.find("status") != string::npos)
        {
            STOMPMessage stompMessage;
            stompMessage.setCommand("SEND");
            stompMessage.addHeader("destination", genre);
            stompMessage.addBody(getUserName() + ":" + _inventory.status(genre));
            string message(stompMessage.get());
            if (!_connectionHandler->sendFrameAscii(message, '\0'))
            {
                cout << "Could not connect to server" << endl;
            }
        }
    }
    if (message.getCommand() == "RECEIPT")
    {
        int id(stoi(message.getHeader("receipt-id")));
        if (_reciepts[id - 1].at(0) == '&')
        {
            _reciepts[id - 1].erase(0, 1);
            cout << "Exited club " + _reciepts[id - 1] << endl;
        }
        else
        {
            _reciepts[id - 1].erase(0, 1);
            if (_reciepts[id - 1] == "logout")
            {
                _connected = false;
                _connectionHandler->close();
            }
            else
            {
                cout << "Joined club " + _reciepts[id - 1] << endl;
            }
        }
    }
    if (message.getCommand() == "ERROR")
    {
        _connected = false;
        cout << message.getBody() << endl;
        _connectionHandler->close();
    }
}
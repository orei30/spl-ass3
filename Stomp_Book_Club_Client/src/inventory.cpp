#include <ineventory.h>

Inventory::Inventory() : _books(), _genres(){};

Inventory::~Inventory() {
    for(pair<std::string, vector<Book*>> genreBooks : _books) {
        for (size_t i(0); i < genreBooks.second.size(); ++i) {
            delete genreBooks.second[i];
        }
        genreBooks.second.clear();
    }
    _books.clear();
    _genres.clear();
};

int Inventory::initializeGenre(string genre)
{
    _genres.push_back(genre);
    if(_books.find(genre)!=_books.end())
        _books.insert({genre, {}});
    return _genres.size() - 1;
}

int Inventory::getGenreId(string genre)
{
    for (size_t i(0); i < _genres.size(); ++i)
        if (_genres[i] == genre)
            return i;
    return -1;
}

void Inventory::exitGenre(string genre) {
    for (size_t i(0); i < _genres.size(); ++i)
    {
        if(_genres[i] == genre)
            _genres.erase (_genres.begin()+i);
    }
}

Book *Inventory::getBook(string genre, string bookName)
{
    vector<Book *> genreBooks(_books.find(genre)->second);
    for (size_t i(0); i < genreBooks.size(); ++i)
        if (genreBooks[i]->getName() == bookName)
            return genreBooks[i];
    return nullptr;
}

bool Inventory::bookValid(string genre, string bookName)
{
    vector<Book *> genreBooks(_books.find(genre)->second);
    for (size_t i(0); i < genreBooks.size(); ++i)
    {
        if (genreBooks[i]->getName() == bookName)
        {
            if (genreBooks[i]->getStatus() == "valid")
                return true;
            else
                return false;
        }
    }
    return false;
}

void Inventory::addBookToGenre(string genre, Book *book)
{
    if (_books.find(genre) == _books.end())
        _books.insert({genre, {book}});
    else
        _books.find(genre)->second.push_back(book);
}

void Inventory::removeBookFromGenre(string genre, Book *book)
{
    vector<Book *> &genreBooks = _books.find(genre)->second;
    for (size_t i(0); i < genreBooks.size(); ++i)
    {
        if (genreBooks[i]->getName() == book->getName())
        {
            delete genreBooks[i];
            genreBooks.erase(genreBooks.begin() + i);
            break;
        }
    }
}

string Inventory::status(string genre)
{
    vector<Book *> &genreBooks = _books.find(genre)->second;
    if (genreBooks.empty())
        return "";
    string status;
    for (size_t i(0); i < genreBooks.size(); ++i)
        if (genreBooks[i]->getStatus() == "valid")
            status = status + genreBooks[i]->getName() + ", ";
    if (status == "")
        return "";
    else
        return status.substr(0, status.size() - 2);
}

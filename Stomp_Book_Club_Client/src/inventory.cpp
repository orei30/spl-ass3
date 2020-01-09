#include <ineventory.h>

Inventory::Inventory() {};

int Inventory::initializeGenre(string genre){
    _genres.push_back(genre);
    _books.insert({genre, {}});
    return _genres.size() - 1;
}

int Inventory::getGenreId(string genre) {
    for(int i(0); i < _genres.size(); ++i) {
        if(_genres[i] == genre) {
            return i;
        }
    }
}

Book Inventory::getBook(string genre, string bookName) {
    vector<Book> genreBooks(_books.find(genre)->second);
    for(int i(0); i < genreBooks.size(); ++i) {
        if(genreBooks[i].getName() == bookName) {
            return genreBooks[i];
        }
    }
}

bool Inventory::bookValid(string genre, string bookName) {
    vector<Book> genreBooks(_books.find(genre)->second);
    for(int i(0); i < genreBooks.size(); ++i) {
        if(genreBooks[i].getName() == bookName) {
            if(genreBooks[i].getStatus() == "valid")
                return true;
            else
                return false;
        }
    }
    return false;
}

void Inventory::addBookToGenre(string genre, Book book) {
    _books.find(genre)->second.push_back(book);
}

void Inventory::removeBookFromGenre(string genre, Book book) {
    vector<Book> genreBooks(_books.find(genre)->second);
    for(int i(0); i < genreBooks.size(); ++i) {
        if(genreBooks[i].getName() == book.getName()) {
            genreBooks.erase(genreBooks.begin()+(i-1));
            break;
        }
    }
}

string Inventory::status(string genre) {
    vector<Book> genreBooks(_books.find(genre)->second);
    string status;
    for(int i(0); i < genreBooks.size() - 1; ++i) {
        status = status + genreBooks[i].getName() + ", ";
    }
    return status + genreBooks[genreBooks.size() - 1].getName();
}

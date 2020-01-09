#ifndef INVENTORY_
#define INVENTORY_

#include <string>
#include <unordered_map>
#include <vector>
#include <book.h>

using namespace std;
class Inventory {
    private:
        unordered_map<string, vector<Book>> _books;
        vector<string> _genres;
    public:
        Inventory();
        int initializeGenre(string);
        int getGenreId(string);
        Book getBook(string, string);
        bool bookValid(string, string);
        void addBookToGenre(string, Book);
        void removeBookFromGenre(string, Book);
        string status(string);
};
#endif
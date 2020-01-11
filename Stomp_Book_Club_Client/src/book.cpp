#include <book.h>

Book::Book(string name, string lender, string genre, string status) : _name(name), _lender(lender), _genre(genre), _status(status) {}

Book::Book() : _name(), _lender(), _genre(), _status() {}

string Book::getName() {
 return _name;
}
void Book::setLender(string lender) {
    _lender = lender;
}
string Book::getLender() {
    return _lender;
}
void Book::getGenre(string genre) {
    genre = genre;
}
void Book::setStatus(string status) {
    _status = status;
}
string Book::getStatus() {
    return _status;
}
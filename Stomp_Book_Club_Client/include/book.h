#ifndef BOOK_
#define BOOK_

#include <string>

using namespace std;
class Book {
    private:
        string _name;
        string _lender;
        string _genre;
        string _status;
    public:
        Book(string, string, string, string);
        string getName();
        void setLender(string);
        string getLender();
        void getGenre(string);
        void setStatus(string);
        string getStatus();
};
#endif
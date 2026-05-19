#define MAX_TITLE    100
#define MAX_AUTHOR   100
#define MAX_CATEGORY  50
#define MAX_LOGIN     50

typedef struct {
    int  id;                      
    char title[MAX_TITLE];         
    char author[MAX_AUTHOR];       
    char category[MAX_CATEGORY];   
    int  available;                
    char borrower[MAX_LOGIN];      
} Book;


void initBook(Book *b, int id, char *title, char *author, char *category);

void displayBook(Book *b);

void displayAvailableBooks(Book *books, int nbBooks);

int searchByTitle(Book *books, int nbBooks, char *title);

int searchByAuthor(Book *books, int nbBooks, char *author);

void searchByCategory(Book *books, int nbBooks, char *category);

void sortByTitle(Book *books, int nbBooks);

void sortByAuthor(Book *books, int nbBooks);

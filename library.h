#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING 256
#define MAX_BOOKS 1000
#define MAX_BORROWED 100
#define USERS_FILE "users.txt"
#define BOOKS_FILE "books.txt"
#define AVAILABLE_BOOKS_FILE "available_books.txt"
#define BORROWED_BOOKS_FILE "borrowed_books.txt"

// Admin credentials (hardcoded)
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// Book structure
typedef struct {
    int id;
    char title[MAX_STRING];
    char author[MAX_STRING];
} Book;

// User structure
typedef struct {
    char username[MAX_STRING];
    char password[MAX_STRING];
} User;

// Borrowed book structure
typedef struct {
    char username[MAX_STRING];
    int book_id;
    char title[MAX_STRING];
    char author[MAX_STRING];
} BorrowedBook;

// Authentication functions
int registerUser();
int loginUser(char *username);
int loginAdmin();
void displayAuthMenu();

// Book file operations
int loadBooks(Book books[], int max_books);
int loadAvailableBooks(Book books[], int max_books);
int saveAvailableBooks(Book books[], int count);
int loadBorrowedBooks(BorrowedBook borrowed[], int max_borrowed);
int saveBorrowedBooks(BorrowedBook borrowed[], int count);
void initializeAvailableBooks();

// User menu functions
void displayAllBooks();
void displayAvailableBooks();
void borrowBook(const char *username);
void returnBook(const char *username);
void displayMyBorrowedBooks(const char *username);
void displayUserMenu(const char *username);

// Utility functions
void clearInputBuffer();
int getIntInput();
void trimString(char *str);

#endif

#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define MAX_ISBN_LEN 20
#define MAX_USERNAME_LEN 50
#define MAX_NAME_LEN 100
#define MAX_PASSWORD_LEN 50
#define FILENAME "library.dat"
#define USERS_FILENAME "users.txt"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// User role enumeration
typedef enum {
    ROLE_USER = 0,
    ROLE_ADMIN = 1
} UserRole;

// Book structure
typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
    bool is_available;
    int borrower_id;  // ID of user who borrowed the book (0 if available)
} Book;

// User structure
typedef struct {
    int id;
    char username[MAX_USERNAME_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
} User;

// Borrow record structure (stored in user-specific files)
typedef struct {
    int book_id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
} BorrowRecord;

// Library structure
typedef struct {
    Book *books;
    int count;
    int capacity;
} Library;

// User management structure
typedef struct {
    User *users;
    int count;
    int capacity;
} UserManager;

// Function declarations

// Library management
Library* library_create(void);
void library_destroy(Library *lib);
int library_add_book(Library *lib, const char *title, const char *author, 
                     const char *isbn, int year);
int library_remove_book(Library *lib, int id);
Book* library_find_book_by_id(Library *lib, int id);
Book* library_find_book_by_title(Library *lib, const char *title);
Book* library_find_book_by_author(Library *lib, const char *author);
Book* library_find_book_by_isbn(Library *lib, const char *isbn);

// File operations
int library_save_to_file(Library *lib, const char *filename);
int library_load_from_file(Library *lib, const char *filename);
int library_load_from_txt(Library *lib, const char *filename);

// Display functions
void library_display_all(Library *lib);
void library_display_book(const Book *book);
void library_display_available(Library *lib);
void library_display_borrowed(Library *lib);

// Book operations
int book_borrow(Library *lib, int book_id, int user_id);
int book_return(Library *lib, int book_id, int user_id);
int book_update(Library *lib, int id, const char *title, const char *author,
                const char *isbn, int year);

// User management
UserManager* user_manager_create(void);
void user_manager_destroy(UserManager *um);
int user_add(UserManager *um, const char *username, const char *name, const char *password, UserRole role);
User* user_find_by_id(UserManager *um, int id);
User* user_find_by_username(UserManager *um, const char *username);
int user_save_to_file(UserManager *um, const char *filename);
int user_load_from_file(UserManager *um, const char *filename);
int get_next_user_id(UserManager *um);

// Authentication
int user_login(UserManager *um, const char *username, const char *password, User **logged_user);
int user_register(UserManager *um, const char *username, const char *name, const char *password);
bool is_admin(const User *user);

// User borrowing records (user-specific files)
int user_borrow_record_add(int user_id, const Book *book);
int user_borrow_record_remove(int user_id, int book_id);
int user_borrow_record_load(int user_id, BorrowRecord **records, int *count);
void user_borrow_record_display(int user_id);
char* get_user_filename(int user_id);

// Validation functions
bool validate_isbn(const char *isbn);
bool validate_year(int year);
bool validate_string(const char *str, int max_len);
int get_next_id(Library *lib);

#endif // LIBRARY_H


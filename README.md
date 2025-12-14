# Library Management System

A comprehensive, modular Library Management System developed in C with persistent storage, input validation, and memory-safe design.

## Features

- **Login/Register System**: Secure authentication with username and password
- **Role-Based Access Control**: 
  - **Admin**: Full access to all features (book management, user management)
  - **Regular Users**: Limited access (search, borrow, return, view own books)
- **Modular Design**: Separated into header (`library.h`), implementation (`library.c`), and main program (`main.c`)
- **Data Structures**: Uses structs for `Book`, `Library`, `User`, and `UserManager` with dynamic memory allocation
- **Persistent Storage**: Mixed file formats for data persistence
  - `library.dat` - Main book catalog (binary)
  - `users.txt` - User registry (text format: id|username|name|password|role)
  - `user_<id>_books.dat` - Individual user borrowing records (binary)
- **User-Based Borrowing System**: Each user has individual borrowing records stored in separate files
- **Exclusive Ownership**: Books can only be borrowed by one user at a time
- **CRUD Operations**: Create, Read, Update, and Delete books and users
- **Search Functionality**: Search by ID, Title, Author, or ISBN
- **Borrow/Return System**: Track book availability and borrower information
- **Input Validation**: Comprehensive validation for all user inputs
- **Memory Safety**: Proper memory allocation and deallocation with null checks
- **Error Handling**: Robust error handling throughout the system

## Data Structures

### Book Structure
```c
typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
    bool is_available;
    int borrower_id;  // ID of user who borrowed the book (0 if available)
} Book;
```

### Library Structure
```c
typedef struct {
    Book *books;
    int count;
    int capacity;
} Library;
```

### User Structure
```c
typedef enum {
    ROLE_USER = 0,
    ROLE_ADMIN = 1
} UserRole;

typedef struct {
    int id;
    char username[MAX_USERNAME_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
} User;
```

### UserManager Structure
```c
typedef struct {
    User *users;
    int count;
    int capacity;
} UserManager;
```

### BorrowRecord Structure
```c
typedef struct {
    int book_id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
} BorrowRecord;
```

## Compilation

### Using Makefile
```bash
make
```

### Manual Compilation
```bash
gcc -Wall -Wextra -std=c11 -pedantic -o library main.c library.c
```

## Usage

Run the program:
```bash
./library
```

Or on Windows:
```bash
library.exe
```

## Login/Register System

When you start the program, you'll see a login screen with options:

1. **Login** - Login with username and password
2. **Register** - Create a new user account
3. **Admin Login** - Login as administrator
   - Default admin credentials: `admin` / `admin123`
4. **Exit** - Exit the program

## Menu Options

### Admin Menu (Full Access)
1. **Add Book** - Add a new book to the library
2. **Search Book** - Search by ID, Title, Author, or ISBN
3. **Update Book** - Update book information
4. **Delete Book** - Remove a book from the library
5. **Display All Books** - Show all books in the library
6. **Display Available Books** - Show only available books
7. **Display Borrowed Books** - Show only borrowed books (with borrower ID)
8. **Add User** - Register a new user (admin only)
9. **Display All Users** - Show all registered users
10. **View User's Borrowed Books** - Display books borrowed by any user
11. **Logout** - Logout and return to login screen
12. **Exit** - Save all data and exit

### Regular User Menu (Limited Access)
1. **Search Book** - Search by ID, Title, Author, or ISBN
2. **Display All Books** - Show all books in the library
3. **Display Available Books** - Show only available books
4. **Borrow Book** - Borrow a book (automatically uses your user ID)
5. **Return Book** - Return a borrowed book (automatically uses your user ID)
6. **View My Borrowed Books** - Display your own borrowed books
7. **Logout** - Logout and return to login screen
8. **Exit** - Save all data and exit

## File Structure

### Source Files
- `library.h` - Header file with struct definitions and function declarations
- `library.c` - Implementation of all library and user management functions
- `main.c` - Main program with menu interface
- `Makefile` - Build configuration

### Data Files (Created Automatically)
- `library.dat` - Main book catalog (binary format)
- `users.txt` - User registry (text format: `id|username|name|password|role`)
  - Format: One user per line, fields separated by `|`
  - Example: `1|john_doe|John Doe|password123|0`
- `user_<id>_books.dat` - Individual user borrowing records (binary format)
  - Example: `user_1_books.dat`, `user_2_books.dat`, etc.

## Memory Management

- Dynamic memory allocation using `malloc()` and `realloc()`
- Automatic resizing when library capacity is exceeded
- Proper cleanup with `free()` in `library_destroy()`
- Null pointer checks throughout the code

## Input Validation

- String length validation
- Year range validation (0-9999)
- ISBN uniqueness checking
- Empty input detection
- Type checking for numeric inputs

## Authentication & Authorization

### Login System
- Users must login before accessing the system
- Passwords are stored in plain text in `users.txt` (for simplicity)
- Admin account has special privileges
- Default admin credentials: `admin` / `admin123`

### Role-Based Access
- **Admin Role**: Can manage books, users, and view all borrowing records
- **User Role**: Can only search books, borrow/return books, and view own records

## User-Based Borrowing System

The system implements a comprehensive user-based borrowing mechanism:

1. **User Registration**: Users must register and login before borrowing books
2. **Exclusive Ownership**: When a book is borrowed:
   - The book's availability status is updated in the main catalog
   - The `borrower_id` field is set to the user's ID
   - A record is added to the user's personal borrowing file (`user_<id>_books.dat`)
3. **Return Process**: When a book is returned:
   - The system verifies the book is borrowed by the logged-in user
   - The book's status is updated in the main catalog
   - The record is removed from the user's borrowing file
4. **Individual Records**: Each user has their own file containing all currently borrowed books
5. **Data Integrity**: The system ensures exclusive ownership - a book can only be borrowed by one user at a time

## Error Handling

- File I/O error handling
- Memory allocation failure handling
- Invalid input detection
- Book not found scenarios
- User not found scenarios
- Duplicate ISBN/username prevention
- Ownership verification (users can only return books they borrowed)
- Exclusive borrowing enforcement (books already borrowed cannot be borrowed again)

## Technical Details

- **Language**: C (C11 standard)
- **File Format**: Binary (.dat)
- **Memory Model**: Dynamic allocation with automatic resizing
- **Platform**: Cross-platform (Windows, Linux, macOS)


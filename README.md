# Library Management System (C)

A console-based Library Management System implemented in C using text files for data storage.

## Features

- **User Authentication**: Register and login system
- **Admin Access**: Separate admin login (hardcoded credentials)
- **Book Management**: View all books, available books, and borrowed books
- **Borrowing System**: Borrow and return books
- **File-based Storage**: All data stored in text files (no database required)

## File Structure

- `library.h` - Header file with structures and function declarations
- `library.c` - Main implementation file
- `books.txt` - Contains all books in the system
- `available_books.txt` - Contains books available for borrowing (auto-initialized from books.txt)
- `borrowed_books.txt` - Contains books currently borrowed by users
- `users.txt` - Contains registered user credentials

## Compilation

### Using GCC (Linux/Mac/Windows with MinGW):

```bash
gcc -o library library.c
```

### Using Clang:

```bash
clang -o library library.c
```

## Running the Program

```bash
./library
```

On Windows:
```bash
library.exe
```

## Default Credentials

**Admin:**
- Username: `admin`
- Password: `admin123`

## Usage

### Authentication Menu

1. **Register** - Create a new user account
2. **Login (User)** - Login as a regular user
3. **Login (Admin)** - Login as administrator
4. **Exit** - Close the program

### User Menu (after login)

1. **Display all books** - Show all books in the system
2. **Display available books** - Show only books available for borrowing
3. **Borrow a book** - Borrow a book by ID
4. **Return a book** - Return a borrowed book
5. **Display my borrowed books** - Show books borrowed by current user
6. **Exit** - Logout and return to main menu

## Data File Formats

### books.txt
```
id;title;author
1;Pride and Prejudice;Jane Austen
2;1984;George Orwell
```

### available_books.txt
```
id;title;author
1;Pride and Prejudice;Jane Austen
```

### borrowed_books.txt
```
username;book_id;title;author
john;2;1984;George Orwell
```

### users.txt
```
username password
john mypassword123
```

## Notes

- The program automatically initializes `available_books.txt` from `books.txt` on first run
- All file operations use semicolon (`;`) as delimiter
- User passwords are stored in plain text (for simplicity - not recommended for production)
- The program handles file I/O errors gracefully

## Requirements

- C compiler (GCC, Clang, or MSVC)
- Standard C library only (no external dependencies)


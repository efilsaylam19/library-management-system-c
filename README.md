# Library Management System (C)

A console-based Library Management System implemented in C using text files for data storage. This project provides a complete solution for managing books, users, and borrowing operations with a clean, modular architecture.

## Features

- **User Authentication**: Secure registration and login system
- **Admin Access**: Separate admin login with hardcoded credentials
- **Book Management**: 
  - View all books in the system
  - View available books for borrowing
  - Track borrowed books with user information
- **Borrowing System**: 
  - Borrow books (removes from available, adds to borrowed)
  - Return books (removes from borrowed, adds back to available)
  - View personal borrowing history
- **File-based Storage**: All data stored in text files (no database required)
- **Input Validation**: Comprehensive validation for all user inputs
- **Error Handling**: Robust error handling throughout the system
- **Portable Code**: Works on Windows, Linux, and macOS

## Project Structure

```
library/
├── library.h          # Header file with structures and function declarations
├── library.c          # Main implementation file (all functions)
├── books.txt          # Contains all books in the system
├── available_books.txt # Contains books available for borrowing
├── borrowed_books.txt  # Contains books currently borrowed by users
├── users.txt          # Contains registered user credentials
├── Makefile           # Build configuration
└── README.md          # This file
```

## Data Structures

### Book Structure
```c
typedef struct {
    int id;
    char title[MAX_STRING];
    char author[MAX_STRING];
} Book;
```

### User Structure
```c
typedef struct {
    char username[MAX_STRING];
    char password[MAX_STRING];
} User;
```

### BorrowedBook Structure
```c
typedef struct {
    char username[MAX_STRING];
    int book_id;
    char title[MAX_STRING];
    char author[MAX_STRING];
} BorrowedBook;
```

## Compilation

### Using Makefile (Recommended)

```bash
make
```

Or on Windows:
```bash
make windows
```

### Using GCC (Linux/Mac/Windows with MinGW):

```bash
gcc -Wall -Wextra -std=c99 -o library library.c
```

### Using Clang:

```bash
clang -Wall -Wextra -std=c99 -o library library.c
```

### Compilation Flags

- `-Wall`: Enable all warnings
- `-Wextra`: Enable extra warnings
- `-std=c99`: Use C99 standard
- `-o library`: Output executable name

## Running the Program

### Linux/Mac:
```bash
./library
```

### Windows:
```bash
library.exe
```

Or simply double-click `library.exe` in File Explorer.

## Default Credentials

**Admin Account:**
- Username: `admin`
- Password: `admin123`

## Usage Guide

### Authentication Menu

When you start the program, you'll see the main authentication menu:

```
========================================
    Library Management System
========================================
1. Register
2. Login (User)
3. Login (Admin)
4. Exit
========================================
```

#### Option 1: Register
- Create a new user account
- Enter username and password
- Account is saved to `users.txt`
- After registration, you can login as a user

#### Option 2: Login (User)
- Login with your registered username and password
- Access the user menu after successful login

#### Option 3: Login (Admin)
- Login with admin credentials (default: `admin` / `admin123`)
- Admin panel access (currently shows placeholder message)

#### Option 4: Exit
- Safely exit the program

### User Menu (After Login)

Once logged in as a user, you'll see:

```
========================================
    Welcome, [username]!
========================================
1. Display all books
2. Display available books
3. Borrow a book
4. Return a book
5. Display my borrowed books
6. Exit
========================================
```

#### Option 1: Display all books
- Shows all books from `books.txt`
- Displays ID, Title, and Author in a formatted table

#### Option 2: Display available books
- Shows only books available for borrowing
- Reads from `available_books.txt`

#### Option 3: Borrow a book
- First displays available books
- Enter the book ID you want to borrow
- The book is:
  - Removed from `available_books.txt`
  - Added to `borrowed_books.txt` with your username
- Prevents borrowing if book is not available

#### Option 4: Return a book
- First displays your borrowed books
- Enter the book ID you want to return
- The book is:
  - Removed from `borrowed_books.txt`
  - Added back to `available_books.txt`
- Only allows returning books you've borrowed

#### Option 5: Display my borrowed books
- Shows only books borrowed by the current user
- Reads from `borrowed_books.txt` and filters by username

#### Option 6: Exit
- Logout and return to authentication menu

## Data File Formats

### books.txt
Contains all books in the system. Format: `id;title;author`

```
1;Pride and Prejudice;Jane Austen
2;1984;George Orwell
3;To Kill a Mockingbird;Harper Lee
4;Moby-Dick;Herman Melville
5;War and Peace;Leo Tolstoy
```

### available_books.txt
Contains books available for borrowing. Format: `id;title;author`

```
1;Pride and Prejudice;Jane Austen
3;To Kill a Mockingbird;Harper Lee
```

**Note:** This file is automatically initialized from `books.txt` on first run.

### borrowed_books.txt
Contains books currently borrowed. Format: `username;book_id;title;author`

```
john;2;1984;George Orwell
jane;4;Moby-Dick;Herman Melville
```

### users.txt
Contains registered user credentials. Format: `username password`

```
john mypassword123
jane securepass456
```

## Technical Details

### File I/O Operations
- All file operations use standard C library functions (`fopen`, `fgets`, `fprintf`, etc.)
- Files are opened in appropriate modes (read, write, append)
- Error handling for file operations

### String Parsing
- Custom `parseField()` function for parsing semicolon-delimited files
- Portable implementation (no POSIX-specific functions)
- Handles whitespace trimming automatically

### Memory Management
- Uses stack-allocated arrays (no dynamic memory allocation)
- Maximum limits defined in `library.h`:
  - `MAX_BOOKS`: 1000
  - `MAX_BORROWED`: 100
  - `MAX_STRING`: 256

### Input Validation
- Integer input validation
- String length checks
- Empty input detection
- Book availability verification
- User ownership verification for returns

### Error Handling
- File I/O error checking
- Invalid input detection
- Book not found scenarios
- User not found scenarios
- Duplicate username prevention

## Code Architecture

The project follows a modular design:

- **library.h**: Header file with all structure definitions, constants, and function declarations
- **library.c**: Implementation file containing:
  - Authentication functions (register, login)
  - File I/O operations (load, save)
  - Book management functions
  - User menu functions
  - Utility functions (input handling, string manipulation)
  - Main function with menu loop

### Key Functions

- `registerUser()`: Handles user registration
- `loginUser()`: Validates user credentials
- `loginAdmin()`: Validates admin credentials
- `loadBooks()`: Reads books from `books.txt`
- `loadAvailableBooks()`: Reads available books
- `saveAvailableBooks()`: Writes available books
- `loadBorrowedBooks()`: Reads borrowed books
- `saveBorrowedBooks()`: Writes borrowed books
- `borrowBook()`: Handles book borrowing logic
- `returnBook()`: Handles book return logic
- `displayUserMenu()`: Main user interface loop

## Important Notes

- The program automatically initializes `available_books.txt` from `books.txt` on first run if it doesn't exist
- All file operations use semicolon (`;`) as delimiter
- User passwords are stored in plain text (for simplicity - **not recommended for production**)
- The program handles file I/O errors gracefully
- Books can only be borrowed by one user at a time
- Users can only return books they have borrowed

## Requirements

- C compiler (GCC, Clang, or MSVC)
- Standard C library only (no external dependencies)
- C99 standard support

## Platform Support

- ✅ Windows (tested with MinGW-W64)
- ✅ Linux
- ✅ macOS

## Future Enhancements

Potential features for future versions:
- Admin panel with full CRUD operations
- Book search functionality
- Due date tracking for borrowed books
- Fine calculation system
- User profile management
- Book categories and tags
- Export/import functionality

## License

This project is open source and available for educational purposes.

## Contributing

Feel free to fork this project and submit pull requests for improvements!

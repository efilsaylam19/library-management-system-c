#include "library.h"

// Global variables (removed unused variable)

// Clear input buffer - clears any leftover characters
void clearInputBuffer() {
    int c;
    // Clear all characters until newline or EOF
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get integer input with validation
int getIntInput() {
    int value;
    char buffer[100];
    char *result;
    
    // Read the entire line including newline - fgets() consumes the newline from stdin
    result = fgets(buffer, sizeof(buffer), stdin);
    if (result == NULL) {
        return -1;
    }
    
    // Check if fgets() read a complete line (ends with newline)
    // If buffer is full and doesn't end with newline, clear the rest of the line
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] != '\n' && len == sizeof(buffer) - 1) {
        // Buffer was filled, clear remaining input
        clearInputBuffer();
    }
    
    // Remove trailing newline, carriage return, and whitespace
    while (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r' || isspace((unsigned char)buffer[len-1]))) {
        buffer[len-1] = '\0';
        len--;
    }
    
    // Try to parse integer from the buffer
    if (sscanf(buffer, "%d", &value) == 1) {
        return value;
    }
    
    return -1;
}

// Trim whitespace from string
void trimString(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    
    while (isspace((unsigned char)str[start])) start++;
    while (end > start && isspace((unsigned char)str[end])) end--;
    
    int i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
}

// Parse semicolon-separated values (portable alternative to strtok_r)
static void parseField(char *line, int field_num, char *output) {
    int i = 0;
    int current_field = 0;
    int start = 0;
    int len = strlen(line);
    
    for (i = 0; i < len; i++) {
        if (line[i] == ';' || line[i] == '\n' || line[i] == '\r') {
            if (current_field == field_num) {
                int j;
                for (j = start; j < i; j++) {
                    output[j - start] = line[j];
                }
                output[j - start] = '\0';
                return;
            }
            start = i + 1;
            current_field++;
        }
    }
    
    // Handle last field
    if (current_field == field_num) {
        int j;
        for (j = start; j < len; j++) {
            output[j - start] = line[j];
        }
        output[len - start] = '\0';
    } else {
        output[0] = '\0';
    }
}

// Register a new user
int registerUser() {
    User new_user;
    FILE *file;
    User existing_user;
    int user_exists = 0;
    
    printf("=== User Registration ===\nEnter username: ");
    fflush(stdout);
    if (fgets(new_user.username, MAX_STRING, stdin) == NULL) {
        return 0;
    }
    trimString(new_user.username);
    
    if (strlen(new_user.username) == 0) {
        printf("Error: Username cannot be empty!\n");
        return 0;
    }
    
    // Check if user already exists
    file = fopen(USERS_FILE, "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s", existing_user.username, existing_user.password) == 2) {
            if (strcmp(existing_user.username, new_user.username) == 0) {
                user_exists = 1;
                break;
            }
        }
        fclose(file);
    }
    
    if (user_exists) {
        printf("Error: Username already exists!\n");
        return 0;
    }
    
    printf("Enter password: ");
    fflush(stdout);
    if (fgets(new_user.password, MAX_STRING, stdin) == NULL) {
        return 0;
    }
    trimString(new_user.password);
    
    if (strlen(new_user.password) == 0) {
        printf("Error: Password cannot be empty!\n");
        return 0;
    }
    
    // Save user to file
    file = fopen(USERS_FILE, "a");
    if (file == NULL) {
        printf("Error: Cannot open users file for writing!\n");
        return 0;
    }
    
    fprintf(file, "%s %s\n", new_user.username, new_user.password);
    fclose(file);
    
    printf("Registration successful!\n");
    return 1;
}

// Login as user
int loginUser(char *username) {
    char input_username[MAX_STRING];
    char input_password[MAX_STRING];
    User user;
    FILE *file;
    
    printf("=== User Login ===\nEnter username: ");
    fflush(stdout);
    // Skip any empty lines (leftover newlines from previous input)
    do {
        if (fgets(input_username, MAX_STRING, stdin) == NULL) {
            return 0;
        }
        trimString(input_username);
    } while (strlen(input_username) == 0);
    
    printf("Enter password: ");
    fflush(stdout);
    if (fgets(input_password, MAX_STRING, stdin) == NULL) {
        return 0;
    }
    trimString(input_password);
    
    file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("Error: Cannot open users file!\n");
        return 0;
    }
    
    while (fscanf(file, "%s %s", user.username, user.password) == 2) {
        if (strcmp(user.username, input_username) == 0 && 
            strcmp(user.password, input_password) == 0) {
            strcpy(username, input_username);
            fclose(file);
            printf("Login successful! Welcome, %s!\n", username);
            return 1;
        }
    }
    
    fclose(file);
    printf("Error: Invalid username or password!\n");
    return 0;
}

// Login as admin
int loginAdmin() {
    char input_username[MAX_STRING];
    char input_password[MAX_STRING];
    
    printf("=== Admin Login ===\nEnter username: ");
    fflush(stdout);
    // Skip any empty lines (leftover newlines from previous input)
    do {
        if (fgets(input_username, MAX_STRING, stdin) == NULL) {
            return 0;
        }
        trimString(input_username);
    } while (strlen(input_username) == 0);
    
    printf("Enter password: ");
    fflush(stdout);
    if (fgets(input_password, MAX_STRING, stdin) == NULL) {
        return 0;
    }
    trimString(input_password);
    
    if (strcmp(input_username, ADMIN_USERNAME) == 0 && 
        strcmp(input_password, ADMIN_PASSWORD) == 0) {
        printf("Admin login successful!\n");
        return 1;
    }
    
    printf("Error: Invalid admin credentials!\n");
    return 0;
}

// Display authentication menu
void displayAuthMenu() {
    printf("\n");
    printf("========================================\n");
    printf("    Library Management System\n");
    printf("========================================\n");
    printf("1. Register\n");
    printf("2. Login (User)\n");
    printf("3. Login (Admin)\n");
    printf("4. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
    fflush(stdout);
}

// Load all books from books.txt
int loadBooks(Book books[], int max_books) {
    FILE *file = fopen(BOOKS_FILE, "r");
    int count = 0;
    char line[MAX_STRING * 3];
    
    if (file == NULL) {
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_books) {
        // Format: id;title;author
        char field[MAX_STRING];
        
        parseField(line, 0, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        books[count].id = atoi(field);
        
        parseField(line, 1, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(books[count].title, field);
        
        parseField(line, 2, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(books[count].author, field);
        
        count++;
    }
    
    fclose(file);
    return count;
}

// Load available books from available_books.txt
int loadAvailableBooks(Book books[], int max_books) {
    FILE *file = fopen(AVAILABLE_BOOKS_FILE, "r");
    int count = 0;
    char line[MAX_STRING * 3];
    
    if (file == NULL) {
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_books) {
        char field[MAX_STRING];
        
        parseField(line, 0, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        books[count].id = atoi(field);
        
        parseField(line, 1, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(books[count].title, field);
        
        parseField(line, 2, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(books[count].author, field);
        
        count++;
    }
    
    fclose(file);
    return count;
}

// Save available books to file
int saveAvailableBooks(Book books[], int count) {
    FILE *file = fopen(AVAILABLE_BOOKS_FILE, "w");
    int i;
    
    if (file == NULL) {
        return 0;
    }
    
    for (i = 0; i < count; i++) {
        fprintf(file, "%d;%s;%s\n", books[i].id, books[i].title, books[i].author);
    }
    
    fclose(file);
    return 1;
}

// Load borrowed books from borrowed_books.txt
int loadBorrowedBooks(BorrowedBook borrowed[], int max_borrowed) {
    FILE *file = fopen(BORROWED_BOOKS_FILE, "r");
    int count = 0;
    char line[MAX_STRING * 4];
    
    if (file == NULL) {
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_borrowed) {
        // Format: username;book_id;title;author
        char field[MAX_STRING];
        
        parseField(line, 0, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(borrowed[count].username, field);
        
        parseField(line, 1, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        borrowed[count].book_id = atoi(field);
        
        parseField(line, 2, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(borrowed[count].title, field);
        
        parseField(line, 3, field);
        trimString(field);
        if (strlen(field) == 0) continue;
        strcpy(borrowed[count].author, field);
        
        count++;
    }
    
    fclose(file);
    return count;
}

// Save borrowed books to file
int saveBorrowedBooks(BorrowedBook borrowed[], int count) {
    FILE *file = fopen(BORROWED_BOOKS_FILE, "w");
    int i;
    
    if (file == NULL) {
        return 0;
    }
    
    for (i = 0; i < count; i++) {
        fprintf(file, "%s;%d;%s;%s\n", 
                borrowed[i].username, 
                borrowed[i].book_id, 
                borrowed[i].title, 
                borrowed[i].author);
    }
    
    fclose(file);
    return 1;
}

// Initialize available_books.txt from books.txt if it doesn't exist
void initializeAvailableBooks() {
    FILE *available_file = fopen(AVAILABLE_BOOKS_FILE, "r");
    if (available_file != NULL) {
        fclose(available_file);
        return; // File exists, no need to initialize
    }
    
    // Copy books.txt to available_books.txt
    FILE *books_file = fopen(BOOKS_FILE, "r");
    if (books_file == NULL) {
        return; // books.txt doesn't exist yet
    }
    
    available_file = fopen(AVAILABLE_BOOKS_FILE, "w");
    if (available_file == NULL) {
        fclose(books_file);
        return;
    }
    
    char line[MAX_STRING * 3];
    while (fgets(line, sizeof(line), books_file) != NULL) {
        fputs(line, available_file);
    }
    
    fclose(books_file);
    fclose(available_file);
}

// Display all books
void displayAllBooks() {
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    int i;
    
    printf("\n=== All Books ===\n");
    if (count == 0) {
        printf("No books found in the system.\n");
        return;
    }
    
    printf("%-5s %-40s %-30s\n", "ID", "Title", "Author");
    printf("----------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-5d %-40s %-30s\n", books[i].id, books[i].title, books[i].author);
    }
    printf("\n");
}

// Display available books
void displayAvailableBooks() {
    Book books[MAX_BOOKS];
    int count = loadAvailableBooks(books, MAX_BOOKS);
    int i;
    
    printf("\n=== Available Books ===\n");
    if (count == 0) {
        printf("No books available at the moment.\n");
        return;
    }
    
    printf("%-5s %-40s %-30s\n", "ID", "Title", "Author");
    printf("----------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-5d %-40s %-30s\n", books[i].id, books[i].title, books[i].author);
    }
    printf("\n");
}

// Borrow a book
void borrowBook(const char *username) {
    Book available_books[MAX_BOOKS];
    BorrowedBook borrowed[MAX_BORROWED];
    int available_count, borrowed_count;
    int book_id, i, j;
    int found = 0;
    
    printf("\n=== Borrow a Book ===\n");
    displayAvailableBooks();
    
    printf("Enter book ID to borrow: ");
    fflush(stdout);
    book_id = getIntInput();
    
    if (book_id <= 0) {
        printf("Error: Invalid book ID!\n");
        return;
    }
    
    // Load available books
    available_count = loadAvailableBooks(available_books, MAX_BOOKS);
    
    // Find the book in available books
    for (i = 0; i < available_count; i++) {
        if (available_books[i].id == book_id) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Error: Book ID %d is not available for borrowing!\n", book_id);
        return;
    }
    
    // Get selected book details
    Book selected_book = available_books[i];
    
    // Load borrowed books
    borrowed_count = loadBorrowedBooks(borrowed, MAX_BORROWED);
    
    // Add to borrowed books
    strcpy(borrowed[borrowed_count].username, username);
    borrowed[borrowed_count].book_id = selected_book.id;
    strcpy(borrowed[borrowed_count].title, selected_book.title);
    strcpy(borrowed[borrowed_count].author, selected_book.author);
    borrowed_count++;
    
    // Remove from available books
    for (j = i; j < available_count - 1; j++) {
        available_books[j] = available_books[j + 1];
    }
    available_count--;
    
    // Save changes
    if (saveAvailableBooks(available_books, available_count) && 
        saveBorrowedBooks(borrowed, borrowed_count)) {
        printf("Successfully borrowed: %s by %s\n", selected_book.title, selected_book.author);
    } else {
        printf("Error: Failed to save changes!\n");
    }
}

// Return a book
void returnBook(const char *username) {
    BorrowedBook borrowed[MAX_BORROWED];
    Book available_books[MAX_BOOKS];
    int borrowed_count, available_count;
    int i, j;
    int book_id;
    int found = 0;
    
    printf("\n=== Return a Book ===\n");
    displayMyBorrowedBooks(username);
    
    printf("Enter book ID to return: ");
    fflush(stdout);
    book_id = getIntInput();
    
    if (book_id <= 0) {
        printf("Error: Invalid book ID!\n");
        return;
    }
    
    // Load borrowed books
    borrowed_count = loadBorrowedBooks(borrowed, MAX_BORROWED);
    
    // Find the book borrowed by this user
    for (i = 0; i < borrowed_count; i++) {
        if (strcmp(borrowed[i].username, username) == 0 && borrowed[i].book_id == book_id) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Error: You haven't borrowed book ID %d!\n", book_id);
        return;
    }
    
    // Save book details before removing
    int return_book_id = borrowed[i].book_id;
    char return_title[MAX_STRING];
    char return_author[MAX_STRING];
    strcpy(return_title, borrowed[i].title);
    strcpy(return_author, borrowed[i].author);
    
    // Load available books
    available_count = loadAvailableBooks(available_books, MAX_BOOKS);
    
    // Add back to available books
    available_books[available_count].id = return_book_id;
    strcpy(available_books[available_count].title, return_title);
    strcpy(available_books[available_count].author, return_author);
    available_count++;
    
    // Remove from borrowed books
    for (j = i; j < borrowed_count - 1; j++) {
        borrowed[j] = borrowed[j + 1];
    }
    borrowed_count--;
    
    // Save changes
    if (saveAvailableBooks(available_books, available_count) && 
        saveBorrowedBooks(borrowed, borrowed_count)) {
        printf("Successfully returned: %s by %s\n", return_title, return_author);
    } else {
        printf("Error: Failed to save changes!\n");
    }
}

// Display user's borrowed books
void displayMyBorrowedBooks(const char *username) {
    BorrowedBook borrowed[MAX_BORROWED];
    int count = loadBorrowedBooks(borrowed, MAX_BORROWED);
    int i;
    int found = 0;
    
    printf("\n=== My Borrowed Books ===\n");
    
    for (i = 0; i < count; i++) {
        if (strcmp(borrowed[i].username, username) == 0) {
            if (!found) {
                printf("%-5s %-40s %-30s\n", "ID", "Title", "Author");
                printf("----------------------------------------------------------------------------\n");
                found = 1;
            }
            printf("%-5d %-40s %-30s\n", borrowed[i].book_id, borrowed[i].title, borrowed[i].author);
        }
    }
    
    if (!found) {
        printf("You haven't borrowed any books yet.\n");
    }
    printf("\n");
}

// Display user menu
void displayUserMenu(const char *username) {
    int choice;
    
    while (1) {
        printf("\n");
        printf("========================================\n");
        printf("    Welcome, %s!\n", username);
        printf("========================================\n");
        printf("1. Display all books\n");
        printf("2. Display available books\n");
        printf("3. Borrow a book\n");
        printf("4. Return a book\n");
        printf("5. Display my borrowed books\n");
        printf("6. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        fflush(stdout);
        choice = getIntInput();
        
        switch (choice) {
            case 1:
                displayAllBooks();
                break;
            case 2:
                displayAvailableBooks();
                break;
            case 3:
                borrowBook(username);
                break;
            case 4:
                returnBook(username);
                break;
            case 5:
                displayMyBorrowedBooks(username);
                break;
            case 6:
                printf("Thank you for using the Library Management System!\n");
                return;
            default:
                printf("Error: Invalid choice! Please try again.\n");
        }
    }
}

// Main function
int main() {
    int choice;
    char username[MAX_STRING];
    
    // Initialize available_books.txt if needed
    initializeAvailableBooks();
    
    while (1) {
        displayAuthMenu();
        choice = getIntInput();
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(username)) {
                    displayUserMenu(username);
                }
                break;
            case 3:
                if (loginAdmin()) {
                    printf("Admin panel features coming soon!\n");
                }
                break;
            case 4:
                printf("Thank you for using the Library Management System!\n");
                exit(0);
            default:
                printf("Error: Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

